#include "opteeprom.h"

COptEeprom::COptEeprom(QObject *parent) :
    QThread(parent)
{
    //init_eeprom();
    m_runtype = WAIT_RUN;
    m_isnewcard = true;
}

int COptEeprom::init_eeprom()
{
    int ret;

    ret = eeprom_open(I2C_DEV, CHIP_ADDR, &m_eep);
    if(ret < 0)
    {
        qDebug("i2c device (AT24C08) open failed !\n");
        return (-1);
    }

    return(0);
}

bool COptEeprom::eeprom_insert()
{
    if(XIoctl::get_gpdat('g',3)>0)
        return false;
    else
        return true;
}

void COptEeprom::run()
{
    QThread::msleep(10000);
    while(1)
    {
        QThread::msleep(100);
        if(eeprom_insert() && m_isnewcard)
        {
            qDebug()<<"insert new eeprom" << QTime::currentTime().toString("hh:mm:ss");
            QThread::msleep(200);


            if(eeprom_read_n(&m_eep,0,1000,&m_code) < 0)
            {
                m_isnewcard = true;//读取失败 重新读
            }
            else
            {
                m_isnewcard = false;
                XArgDecode newcode(m_code);
                if(newcode.isvalid())
                {
                    emit SigNewBatch(newcode.getPDbatch());
                    int insert = 0;
                    QSqlQuery query = CDataBase::select_Record(BATCH_BODY_NAME,"BATCH",newcode.getPDbatch());
                    if(query.next())
                    {
                        if(!newcode.isIESser())
                        {
                            continue;
                        }
                        else
                            insert = query.value(0).toInt();
                    }
                    else
                        insert = CDataBase::get_InsertAdss(BATCH_BODY_NAME);

                    QList<QVariant> code;
                    code.append(insert);
                    code.append(newcode.getPDbatch());
                    code.append(newcode.get_codestr());

                    if(CDataBase::record_IsExist(BATCH_BODY_NAME, "SERNUM", insert)) //记录存在
                    {
                        CDataBase::del_Record(BATCH_BODY_NAME, "SERNUM", insert);
                    }
                    CDataBase::add_Record(BATCH_BODY_NAME, code);
                }

            }

        }
        if((m_isnewcard == false) && !eeprom_insert())
        {
            m_isnewcard = true;
        }
        //qDebug()<<"-----------------"<<eeprom_insert();
    }
}

__s32 COptEeprom::i2c_smbus_access(int file, char read_write, __u8 command, int size, i2c_smbus_data *data)
{
    struct i2c_smbus_ioctl_data args;

    args.read_write = read_write;
    args.command = command;
    args.size = size;
    args.data = data;
    return ioctl(file, I2C_SMBUS, &args);
}

int COptEeprom::eeprom_read_byte(eeprom *e, __u16 mem_addr)
{
    int r;
    union i2c_smbus_data data;

    ioctl(e->fd, BLKFLSBUF);

    __u8 buf =  mem_addr & 0x0ff;
    r = i2c_smbus_access(e->fd, I2C_SMBUS_WRITE, buf, I2C_SMBUS_BYTE, NULL);;
    if (r < 0)
        return r;
    if (i2c_smbus_access(e->fd, I2C_SMBUS_READ, 0, I2C_SMBUS_BYTE, &data))
        return -1;
    else
        return 0x0FF & data.byte;
}

int COptEeprom::eeprom_write_byte(eeprom *e, __u16 mem_addr, __u8 data)
{
    int r;
    __u8 command = mem_addr & 0x00ff;
    union i2c_smbus_data i2cdata;
    i2cdata.byte = data;
    r = i2c_smbus_access(e->fd, I2C_SMBUS_WRITE, command, I2C_SMBUS_BYTE_DATA, &i2cdata);

    if(r < 0)
        qDebug("iic write error");

    usleep(10);
    return r;
}

int COptEeprom::eeprom_open(char *dev_name, int addr, eeprom *e)
{
    int funcs, fd, r;
    e->fd = e->addr = 0;
    e->dev = 0;

    fd = open(dev_name, O_RDWR);
    if(fd <= 0)
    {
        qDebug("iic open error\n");
        return -1;
    }

    // set working device
    ioctl(fd, I2C_TENBIT, 0);
    ioctl(fd, I2C_SLAVE, addr);

    e->fd = fd;
    e->addr = addr;
    e->dev = dev_name;
    return 0;
}

int COptEeprom::eeprom_close(eeprom *e)
{
    close(e->fd);
    e->fd = -1;
    e->dev = 0;
    return 0;
}

int COptEeprom::eeprom_read_n(eeprom *e, int addr, int size, QByteArray *buf)
{
    /**/
    struct eeprom ep;
    int ret;
    ret = eeprom_open(I2C_DEV, 0x50, &ep);
    if(ret < 0)
    {
        printf("i2c device (AT24C02) open failed !\n");
        return (-1);
    }
    buf->clear();
    for(int page=0;page<4;page++)
    {
        ioctl(ep.fd, I2C_SLAVE, 0x50+page);

        int temp, i;
        for(i = 0; i < 256; ++i)
        {
            temp = eeprom_read_byte(&ep, i);
            if(temp < 0)
            {
                printf("iic read error !");
                return -1;
            }

            if( (i % 16) == 0 )
                printf("\n %.4x|  ", i+page<<8);
            else if( (i % 8) == 0 )
                printf("  ");
            printf("%.2x ", (unsigned int)temp);

            fflush(stdout);

            buf->append(char(temp&0xff));
            if(buf->length() > 4)
            {
                /**/
                if(buf->at(0) != 'B')
                {
                    eeprom_close(&ep);
                    printf("this chip no biotime\n");
                    return -1;
                }

                int datalen = buf->at(2);
                datalen = datalen*256+buf->at(3);
                if(datalen <= buf->length())
                {
                    eeprom_close(&ep);
                    printf("chip read over len=%d \n",datalen);
                    return 0;
                }
            }
        }
    }
    eeprom_close(&ep);
    return 0;

    /*
    int temp, i;
    for(i = 0; i < size; ++i)
    {
        temp = eeprom_read_byte(e, addr+i);
        if(temp < 0)
        {
            qDebug("iic read error !");
            return -1;
        }

        if( (i % 16) == 0 )
                printf("\n %.4x|  ", addr+i);
        else if( (i % 8) == 0 )
                printf("  ");
        printf("%.2x ", temp);

        buf->append(char(temp&0xff));
        fflush(stdout);
    }
*/

    return 0;
}

int COptEeprom::eeprom_write_n(eeprom *e, int addr, int size, QByteArray buf)
{
    int i, ret;
    if(buf.length()<size) size = buf.length();
    for(i=0; i<size; i++)
    {
        fflush(stdout);
        ret = eeprom_write_byte(e, addr+i, buf.at(i));
        if(ret < 0)
        {
            qDebug("iic write error !");
        }
    }
    fprintf(stderr, "\n\n");
    return 0;
}

int COptEeprom::read_from_eeprom(int addr, int size, QByteArray *buf)
{

    m_runtype = READ_RUN;
}

int COptEeprom::write_to_eeprom(int addr, int size, QByteArray buf)
{
}


