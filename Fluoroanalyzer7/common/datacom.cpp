#include "datacom.h"
#include "common/configurate.h"
#include "common/commoninterface.h"


CDataCom::CDataCom(QObject *parent): QThread(parent)
{
    m_Datahead.append(START_PCK_STATE & 0xFF);
    m_Datahead.append(START_PCK_STATE >> 8 &  0xFF);
    m_Datahead.append(START_PCK_STATE >> 16 & 0xFF);
    m_Datahead.append(START_PCK_STATE >> 24 & 0xFF);

    m_Dataend.append(END_PCK_STATE & 0xFF);
    m_Dataend.append(END_PCK_STATE >> 8 & 0xFF);
    m_Dataend.append(END_PCK_STATE >> 16 & 0xFF);
    m_Dataend.append(END_PCK_STATE >> 24 & 0xFF);

    m_pSerialPort = CSerialPort::GetSerialPort(CSerialPort::UART_1_PC_PRINTER);
    m_pSerialPort->setTimeout(5);

    m_qByData.clear();
    m_ReNewPack = false;
    m_qSdData.clear();
    m_qSdBuff.clear();

    //mutex.unlock();

    m_sleep = 100;
    myJiffy = 0;
    b_COMConnect = false;
    m_runtype = COM_RUN;
}

void CDataCom::run()
{
    while(1)
    {
        QThread::msleep(m_sleep);

        //        qDebug() << "jiffy =" << ++myJiffy;

        switch (m_runtype)
        {
        case COM_RUN:
            SlotRdLoop();
            SlotWdLoop();
            break;
        case WAIT_RUN:
            QThread::msleep(100);
            break;
        default:
            break;
        }
    }
}

QByteArray  CDataCom::intToByte(int i)
{
    QByteArray abyte0;
    abyte0.resize(4);
    abyte0[0] = (uchar) 0xff & i;
    abyte0[1] = (uchar) 0xff & (i >> 8);
    abyte0[2] = (uchar) 0xff & (i >> 16);
    abyte0[3] = (uchar) 0xff & (i >> 24);
    return abyte0;
}

int CDataCom::ByteToint(QByteArray ba)
{
    int it=0;

    if(ba.length()<4) return -1;

    for(int i=3;i>=0;i--)
    {
        it<<=8;
        it+=(ba.at(i)&0xff);
    }
    //qDebug("int %x",it);
    return it;
}


int CDataCom::getCRC_BT(DATA_COM *data)
{
    int recrc=0;
    QByteArray tmpData;
    tmpData.append(intToByte(data->comT));
    tmpData.append(intToByte(data->dataT));
    tmpData.append(intToByte(data->packN));
    tmpData.append(intToByte(data->currN));
    tmpData.append(intToByte(data->data_length));
    tmpData.append(data->data);
    recrc = qChecksum(tmpData.constData(),tmpData.length());
    return recrc;
}

bool CDataCom::SendData(QByteArray &data, COM_TYPE comT, DATA_TYPE dataT)
{
    if(!b_COMConnect)
        return false;

    SEND_ST * tmp = new SEND_ST;
    tmp->comT = comT;
    tmp->dataT = dataT;
    tmp->data.append(data);

    m_qSdData.append(tmp);

    return true;
}

bool CDataCom::orgSendData(DATA_COM *send ,QByteArray *data, COM_TYPE comT, DATA_TYPE dataT, int packn, int currn)
{
    send->head = START_PCK_STATE;
    send->comT = comT;
    send->dataT = dataT;
    send->packN = packn;
    send->currN = currn;

    send->data_length = data->length();
    if(data->length()>0)
        send->data.append(*data);
    else
        send->data.clear();

    send->CRC_BT = getCRC_BT(send);
    send->end = END_PCK_STATE;

    return true;
}

bool CDataCom::subPackageData(SEND_ST * ts)
{
    //mutex.lock();

    QByteArray tmpdata = ts->data;
    int packn = tmpdata.length() / FRAME_MAX_LEN + 1;
    qDebug() << "subPackageData() packn=" << packn;

    for(int i = 0; i < packn; i++)
    {
        QByteArray senddata;

        if(tmpdata.length() > FRAME_MAX_LEN)
        {
            senddata.append(tmpdata.left(FRAME_MAX_LEN));
            tmpdata.remove(0, FRAME_MAX_LEN);
        }
        else
        {
            senddata.append(tmpdata);
        }

        DATA_COM * send = new DATA_COM;
        orgSendData(send, &senddata, ts->comT, ts->dataT, packn, i);
        m_qSdBuff.append(send);
    }

    return true;
    //mutex.unlock();
}

bool CDataCom::anaRecvData(DATA_COM *data)
{
    if(data == NULL)
        return false;

    if (data->comT != COM_HBT)
        qDebug("recv comT:0x%X, dataT:0x%X, Total:%u, Curr:%u, Len=%u"
               , data->comT
               , data->dataT
               , data->packN
               , data->currN
               , data->data_length);

    switch(data->comT)
    {
    case COM_CNT:
        RecvConnect(data->dataT);
        break;
    case COM_HBT:
        RecvHeartBeat();
        break;
    case COM_GET:
        RecvGetCom(data);
        break;
    case COM_SET:
        RecvSetCom(data);
        break;
    case COM_CMD:
        RecvCmdCom(data);
        break;
    case COM_UPG:
        RecvUpgCom(data);
        break;
    case COM_RPT:
        //        qDebug()<<"COM_RPT ok";
        break;
    default:
        break;
    }

    return true;
}

bool CDataCom::DataToSend(DATA_COM * data, QByteArray *send)
{
    if (data->comT != COM_HBT)
        qDebug("sned comT:0x%X, dataT:0x%X, Total:%u, Curr:%u, Len=%u"
               , data->comT
               , data->dataT
               , data->packN
               , data->currN
               , data->data_length);

    send->append(intToByte(data->head));
    send->append(intToByte(data->comT));
    send->append(intToByte(data->dataT));
    send->append(intToByte(data->packN));
    send->append(intToByte(data->currN));
    send->append(intToByte(data->data_length));
    send->append(data->data);
    send->append(intToByte(data->CRC_BT));
    send->append(intToByte(data->end));

    return true;
}

bool CDataCom::RecvToData(DATA_COM *data, QByteArray *recData)
{
    if((data == 0) || (recData->length() == 0))
        return false;

    data->head = ByteToint(recData->left(4));
    data->comT = ByteToint(recData->left(8).right(4));
    data->dataT = ByteToint(recData->left(12).right(4));
    data->packN = ByteToint(recData->left(16).right(4));
    data->currN = ByteToint(recData->left(20).right(4));
    data->data_length = ByteToint(recData->left(24).right(4));

    if(data->data_length > (recData->length()-32))
        return false;

    data->data.append(recData->left(24+data->data_length).right(data->data_length));
    data->CRC_BT = ByteToint(recData->right(8).left(4));
    data->end = ByteToint(recData->right(4));

    if(getCRC_BT(data) != data->CRC_BT)
        return false;

    return true;
}

bool CDataCom::FileToStruct(QByteArray &name, QByteArray &data, FILE_ST *file)
{
    QByteArray md5;
    md5 = QCryptographicHash::hash(data, QCryptographicHash::Md5 );
    file->file_name.append(name);
    file->name_len = name.length();
    file->file_data.append(data);
    file->data_len = data.length();
    file->file_MD5.append(md5);

    return true;
}

bool CDataCom::StructToSend(FILE_ST &file, QByteArray *send)
{
    send->append(intToByte(file.name_len));
    send->append(file.file_name);
    send->append(intToByte(file.data_len));
    send->append(file.file_data);
    send->append(file.file_MD5);

    return true;
}

bool CDataCom::FileToSend(QByteArray &name, QByteArray &data, QByteArray *send)
{
    FILE_ST file;
    FileToStruct(name,data,&file);
    StructToSend(file,send);

    return true;
}

bool CDataCom::DataToStruct(QByteArray &data, FILE_ST *file)
{
    file->name_len = ByteToint(data.left(4));

    if(file->name_len>0)
        file->file_name.append(data.left(4+file->name_len).right(file->name_len));

    file->data_len = ByteToint(data.left(4+file->name_len+4).right(4));

    if(file->data_len>0)
        file->file_data.append(data.left(4+file->name_len+4+file->data_len).right(file->data_len));

    file->file_MD5.append(data.right(16));

    return true;
}

bool CDataCom::isSTFileTrue(FILE_ST &file)
{
    QByteArray md5;
    md5 = QCryptographicHash::hash(file.file_data, QCryptographicHash::Md5);

    if(md5 != file.file_MD5)
        return false;

    return true;
}

bool CDataCom::RecvConnect(int dtp)
{
    if(dtp == DATA_CNNT)
    {
        b_COMConnect = true;
        ResponseSucc(COM_CNT);
        emit SigCnntSucc(true);
        m_sleep = 30;

        qDebug() << "connect PC" << QTime::currentTime().toString("hh:mm:ss");

        m_qSdData.clear();
        m_qSdBuff.clear();
        CCommonInterface::get_optbuzzer()->ringdidi();  //debug
    }
    else if(dtp == DATA_BREK)
    {
        b_COMConnect = false;
        emit SigCnntSucc(false);
        m_sleep = 100;

        qDebug() << "disconnect PC" << QTime::currentTime().toString("hh:mm:ss");
    }
}

bool CDataCom::RecvHeartBeat()
{
    ResponseSucc(COM_HBT);
}

bool CDataCom::ResponseSucc(COM_TYPE comT)
{
    QByteArray data;
    DATA_COM send;
    orgSendData(&send, &data, comT, DATA_SUCC, 1, 0);

    //mutex.lock();
    QByteArray senddata;
    DataToSend(&send, &senddata);
    m_pSerialPort->write(senddata);
    //mutex.unlock();

    return true;
}

bool CDataCom::ResponseErr(COM_TYPE comT)
{
    QByteArray data;
    DATA_COM * send = new DATA_COM;
    orgSendData(send, &data, comT, DATA_DERR, 1, 0);

    //mutex.lock();
    m_qSdBuff.append(send);
    //mutex.unlock();

    return true;
    //return SendData(&data,comT,DATA_DERR);
}

bool CDataCom::ConnectToSerPort()
{
    bool bOpen = false;
    return bOpen;
}

bool CDataCom::BreakToSerPort()
{
    return false;
}

bool CDataCom::isBuildCommt()
{
    return b_COMConnect;
}

void CDataCom::SlotRdLoop()
{
    if(m_pSerialPort == NULL)
        return;

    int nRetVar = m_pSerialPort->read(m_recvBuf, 1024);
    if(nRetVar > 0)
    {
        m_qByData.append(m_recvBuf, nRetVar);
        //        qDebug() << "recv pc" << nRetVar << ", buf len =" << m_qByData.length();

        int findAt = -1;

        do{
            //            qDebug() << "m_ReNewPack" << m_ReNewPack;

            if( ! m_ReNewPack)
            {
                findAt = m_qByData.indexOf(m_Datahead);//查找包头
                //                qDebug() << "head" << findAt;

                if(findAt >= 0)
                {
                    m_ReNewPack = true;
                    m_qByData.remove(0, findAt);
                }
                else if(m_qByData.length() >= 1024)
                {
                    m_qByData.remove(0, 1020);
                    qDebug() << "not found head, but buf too much! clear buf 0-1020";
                }
            }

            if(m_ReNewPack)
            {
                findAt = -1;
                findAt = m_qByData.indexOf(m_Dataend);//查找包尾
                //                qDebug() << "tail" << findAt;

                if(findAt >= 0)
                {
                    QByteArray pkgBuf = m_qByData.left(findAt + 4);
                    m_ReNewPack = false;
                    m_qByData.remove(0, findAt + 4);
                    DATA_COM pkg;

                    if(RecvToData(&pkg, &pkgBuf))
                        anaRecvData(&pkg);
                }
                else
                {
                    break;
                }
            }

        } while((m_qByData.indexOf(m_Datahead) >= 0) && (m_qByData.indexOf(m_Dataend) >= 0));

        m_beforetime = QTime::currentTime();
    }

    //超过20秒未收到任何信号，表示连接已经断开.上位机线程间隔4S可接收到一个心跳
    if(isBuildCommt() && (m_beforetime.msecsTo(QTime::currentTime()) >= 20000))
    {
        CCommonInterface::get_optbuzzer()->ringdi();  //debug
        qDebug() << "m_beforetime =" << m_beforetime.toString("hh:mm:ss") << ", now =" << QTime::currentTime().toString("hh:mm:ss");

        m_qSdData.clear();
        m_qSdBuff.clear();
        m_sleep = 100;
        b_COMConnect = false;
        m_qByData.clear();
        emit SigCnntSucc(false);
    }
}

void CDataCom::SlotWdLoop()
{
    if (m_pSerialPort == NULL)
        return;

    //    if(isBuildCommt() && (m_pSerialPort != NULL))
    //    {
    if(m_qSdData.count()>0)
    {
        qDebug() << "convert m_qSdData to m_qSdBuff";

        if (subPackageData(m_qSdData.first()))
        {
            qDebug() << "convert succ, del point";
            delete m_qSdData.first();
            m_qSdData.removeFirst();
            qDebug() << "m_qSdData done";
        }
    }

    if((m_qSdBuff.count() > 0))    //(m_pSerialPort->bytesToWrite() == 0) &&
    {
        QByteArray senddata;
        DataToSend(m_qSdBuff.first(), &senddata);

        m_pSerialPort->write(senddata);

        qDebug() << "send succ, del point";
        delete m_qSdBuff.first();
        m_qSdBuff.removeFirst();
        qDebug() << "m_qSdBuff done";
    }
    //    }
    //    else
    //    {
    //        m_qSdData.clear();
    //        m_qSdBuff.clear();
    //    }
}

bool CDataCom::RecvSetCom(DATA_COM *data)
{
    switch(data->dataT)
    {
    case DATA_BARC:
    {
        if(data->currN == 0)
        {
            //            qDebug()<<"RecvSetCom new code ";
            m_recvcode.clear();
        }
        m_recvcode.append(data->data);
        if((data->currN+1) == data->packN)
        {
            //            qDebug()<<"RecvSetCom succ";
            XArgDecode newcode(m_recvcode);
            if(newcode.isvalid())
            {
                int insert = 0;
                QSqlQuery query = CDataBase::select_Record(BATCH_BODY_NAME,"BATCH",newcode.getPDbatch());
                if(query.next())
                {
                    //                    qDebug()<<"RecvSetCom cui zai";
                    if(!newcode.isIESser())
                    {
                        break;
                    }
                    else
                        insert = query.value(0).toInt();
                }
                else
                    insert = CDataBase::get_InsertAdss(BATCH_BODY_NAME);

                //                qDebug()<<"RecvSetCom updata code insert = "<<insert;

                QList<QVariant> code;
                code.append(insert);
                code.append(newcode.getPDbatch());
                code.append(m_recvcode);

                if(CDataBase::record_IsExist(BATCH_BODY_NAME, "SERNUM", insert)) //记录存在
                {
                    CDataBase::del_Record(BATCH_BODY_NAME, "SERNUM", insert);
                }
                CDataBase::add_Record(BATCH_BODY_NAME, code);

                emit SigNewBatch(newcode.getPDbatch());
            }
        }
        ResponseSucc(COM_SET);
    }
    break;
    case DATA_TIME:
    {
        char cOne = '"';
        QString strCommand;
        strCommand.append("date -s ");
        strCommand.append(cOne);
        strCommand.append(data->data);
        strCommand.append(cOne);

        qDebug() << "system = "
                 << system(strCommand.toStdString().c_str());
        usleep(100);
        system("hwclock -w");
    }
    break;
    case DATA_IFEI:
    {
        CSysInfo::set_cfg(CFG_QT_SNCODE, data->data);
    }
    break;
    default:
        break;
    }

    return true;
}

bool CDataCom::RecvCmdCom(DATA_COM *data)
{
    switch(data->dataT)
    {
    case DATA_STAT:
    {

    }
    break;
    case DATA_STOP:
    {

    }
    break;
    case DATA_REST:
    {
        system("reboot");
    }
    break;
    case DATA_RECO:
    {

    }
    break;
    case DATA_TLAB:
    {

    }
    break;
    case DATA_DELN:
    {
        DEL_PACKAGE del;
        del.com = ByteToint(data->data.left(4));
        del.pos = ByteToint(data->data.left(8).right(4));
        del.num = ByteToint(data->data.left(12).right(4));
        //        qDebug() << "DATA_DELN = "<<del.com<<del.pos<<del.num;
        switch(del.com)
        {
        case DATA_BARC:
        {
            if(del.pos < 0) break;
            for(int i=0;i<del.num;i++)
            {
                if(CDataBase::record_IsExist(BATCH_BODY_NAME, "SERNUM", del.pos+i)) //记录存在
                {
                    CDataBase::del_Record(BATCH_BODY_NAME, "SERNUM", del.pos+i);
                }
            }
        }
        break;
        case DATA_HIST:
        {
            if(del.pos < 0) 
                break;

            for(int i=0;i<del.num;i++)
            {
                if(CDataBase::record_IsExist(TABLE_BODY_NAME, "SERNUM", del.pos+i)) //记录存在
                {
                    CDataBase::del_Record(TABLE_BODY_NAME, "SERNUM", del.pos+i);
                    if(CDataBase::record_IsExist(FLUDT_BODY_NAME, "SERNUM", del.pos+i)) //记录存在
                    {
                        CDataBase::del_Record(FLUDT_BODY_NAME, "SERNUM", del.pos+i);
                    }
                }
            }
        }
        break;
        default:
            break;
        }

    }
    break;
    default:
        break;
    }

    return true;
}

bool CDataCom::getBeginEnd(int * pBegin, int * pEnd, const char * dbTable)
{
    int nDataMax = CDataBase::get_MaxAdss(dbTable);
    int nMaxLimit = CDataBase::get_RecordMax(dbTable);
    qDebug() << dbTable << "nDataMax =" << nDataMax << ", nMaxLimit =" << nMaxLimit;

    int dataEndId = nDataMax;
    int dataBeginId = ((nDataMax - nMaxLimit) > 0) ? (nDataMax - nMaxLimit) : 0;
    qDebug() << "dataBeginId =" << dataBeginId << ", dataEndId =" << dataEndId;

    if(*pEnd < 0) // all data
    {
        *pBegin = dataBeginId;
        *pEnd = dataEndId;
    }
    else
    {
        if ((dataBeginId <= *pEnd) && (*pEnd <= dataEndId))
        {
            if (*pBegin < dataBeginId)
                *pBegin = dataBeginId;
        }
        else
        {
            qDebug() << "getBeginEnd()1";
            return false;
        }
    }

    if (*pBegin > *pEnd)
    {
        qDebug() << "getBeginEnd()2";
        return false;
    }

    qDebug() << "begin = " << *pBegin << ", end =" << *pEnd;

    return true;
}

bool CDataCom::RecvGetCom(DATA_COM *data)
{
    switch(data->dataT)
    {
    case DATA_FLUO:
    {
        int beginId = ByteToint(data->data.left(4));
        int endId = beginId + ByteToint(data->data.left(8).right(4)) - 1;
        qDebug() << "beginId =" << beginId << ", endId =" << endId;

        if ( ! getBeginEnd(&beginId, &endId, FLUDT_BODY_NAME))
            return false;

#if 0
        for(int i = beginId; i <= endId; i++, heartBeat++)
        {
            QSqlQuery query = CDataBase::select_Record(FLUDT_BODY_NAME,"SERNUM", i);
            if(query.next())
            {
                QByteArray send;
                char a[5];
                memcpy(a, &i, 4);
                send.append(a,4);//流水号
                send.append(query.value(1).toByteArray());
                SEND_ST tmp;
                tmp.comT = COM_RPT;
                tmp.dataT = DATA_FLUO;
                tmp.data.append(send);
                subPackageData(&m_qSdBuff,tmp);
            }
        }
#else
        int heartBeat = 0;
        int sernum = beginId;

        QSqlQuery query(tr("select * from " FLUDT_BODY_NAME " where SERNUM>=%1 and SERNUM<=%2").arg(beginId).arg(endId));
        if ( ! query.exec() )
        {
            qDebug() << query.lastError();
            return false;
        }

        char a[5] = {0};
        SEND_ST tmp;
        tmp.comT = COM_RPT;
        tmp.dataT = DATA_FLUO;
        while(query.next())
        {
            tmp.data.clear();
            memcpy(a, &sernum, 4);
            sernum++;
            tmp.data.append(a, 4);//流水号
            tmp.data.append(query.value(1).toByteArray());

            subPackageData(&tmp);

            if (++heartBeat > 500)
            {
                heartBeat = 0;
                RecvHeartBeat();
            }
        }
#endif
    }
    break;
    case DATA_BARC:
    {
        int ser = ByteToint(data->data.left(4));
        int num = ByteToint(data->data.left(8).right(4));
        if(num < 0)
        {
#if 0
            num = CDataBase::get_InsertAdss(BATCH_BODY_NAME);
            for(int i=0;i<num;i++)
            {
                QSqlQuery query = CDataBase::select_Record(BATCH_BODY_NAME,"SERNUM",i);
                if(query.next())
                {
                    SEND_ST tmp;
                    tmp.comT = COM_RPT;
                    tmp.dataT = DATA_BARC;
                    tmp.data.append(query.value(2).toByteArray());
                    subPackageData(&m_qSdBuff,tmp);
                }
            }
#else
            int heartBeat = 0;

            QSqlQuery query(tr("select * from " BATCH_BODY_NAME));
            if ( ! query.exec() )
                qDebug() << query.lastError();

            SEND_ST tmp;
            tmp.comT = COM_RPT;
            tmp.dataT = DATA_BARC;
            while(query.next())
            {
                tmp.data.clear();
                tmp.data.append(query.value(2).toByteArray());
                subPackageData(&tmp);

                if (++heartBeat > 500)
                {
                    heartBeat = 0;
                    RecvHeartBeat();
                }
            }
#endif
        }
        else
        {
            QList<QString> btlist;
            for(int i=0;i<num;i++)
            {
                int cur = ser+i;
                QSqlQuery query = CDataBase::select_Record(TABLE_BODY_NAME,"SERNUM",cur);
                if(query.next())
                {
                    QString batch = query.value(1).toString();
                    if(btlist.indexOf(batch) < 0)
                    {
                        btlist.append(batch);
                    }
                }
            }
            //            qDebug()<<"RecvGetCom "<<btlist;
            for(int i=0;i<btlist.count();i++)
            {
                QSqlQuery query = CDataBase::select_Record(BATCH_BODY_NAME,"BATCH",btlist.at(i));
                if(query.next())
                {
                    SEND_ST tmp;
                    tmp.comT = COM_RPT;
                    tmp.dataT = DATA_BARC;
                    tmp.data.append(query.value(2).toByteArray());
                    subPackageData(&tmp);
                }
            }
        }
    }
    break;
    case DATA_HIST:
    {
        int ser = ByteToint(data->data.left(4));
        int num = ByteToint(data->data.left(8).right(4));
        if(num < 0)
        {
            ser = 1;
            num = CDataBase::get_InsertAdss(TABLE_BODY_NAME);
        }
#if 0
        for(int i=0;i<num;i++)
        {
            QByteArray td;
            if(orgHistData(ser+i,&td))
            {
                SEND_ST tmp;
                tmp.comT = COM_RPT;
                tmp.dataT = DATA_HIST;
                tmp.data.append(td);
                subPackageData(&m_qSdBuff,tmp);
            }
        }
#else
        orgHistDatas(ser, ser + num);
#endif
    }
    break;
    case DATA_PARA:
    {

    }
    break;
    case DATA_VERS:
    {
        SEND_ST tmp;
        tmp.comT = COM_RPT;
        tmp.dataT = DATA_VERS;
        tmp.data.append(CURR_SYS_VERSIONS);
        subPackageData(&tmp);
    }
    break;
    case DATA_IFEI:
    {
        QString sn= CSysInfo::get_cfg(CFG_QT_SNCODE).toString();

        SEND_ST tmp;
        tmp.comT = COM_RPT;
        tmp.dataT = DATA_IFEI;
        tmp.data.append(sn);
        subPackageData(&tmp);
    }
    break;
    case DATA_TIME:
    {
        SEND_ST tmp;
        tmp.comT = COM_RPT;
        tmp.dataT = DATA_TIME;
        tmp.data.append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toAscii());
        subPackageData(&tmp);
    }
    break;
    case DATA_SPCE:
    {
        SEND_ST tmp;
        tmp.comT = COM_RPT;
        tmp.dataT = DATA_SPCE;
        tmp.data.append("");
        subPackageData(&tmp);
    }
    break;
    case DATA_CCFG:
    {
        QString tempfile = "/BioTime/Option.ini";

        QFile fl(tempfile);
        if(fl.open(QFile::ReadOnly))
        {
            QByteArray fba = fl.readAll();
            QByteArray fne = "Option.ini";
            FILE_ST fst;
            QByteArray send;
            FileToStruct(fne,fba,&fst);
            StructToSend(fst,&send);
            //            qDebug()<<fst.data_len;
            //            qDebug()<<fst.file_name;
            //            qDebug()<<fst.file_MD5.toHex()<<fst.file_MD5.length();
            //            qDebug()<<isSTFileTrue(fst);
            SEND_ST tmp;
            tmp.comT = COM_RPT;
            tmp.dataT = DATA_CCFG;
            tmp.data.append(send);
            subPackageData(&tmp);

            fl.close();
        }
    }
    break;
    default:
        break;
    }

    return true;
}

bool CDataCom::RecvUpgCom(DATA_COM *data)
{
    bool recvstate = false;
    ResponseSucc(COM_UPG);
    QString tempfile = tr("/BT_ARM_FILE_%0").arg(DATA_CARM >> 24);
    QFile file(tempfile);

    if(file.open(QIODevice::ReadWrite))
    {
        if(data->currN == 0)
        {
            file.seek(0);
        }
        else
        {
            file.seek(file.size());
        }
        file.write(data->data);
        file.close();
    }

    FILE_ST rcvf;

    if((data->currN + 1) == data->packN)
    {
        if(file.open(QIODevice::ReadOnly))
        {
            QByteArray rd = file.readAll();
            DataToStruct(rd,&rcvf);

            if(isSTFileTrue(rcvf))
            {
                system("rm -f /Option.ini;");
                QFile upf("/"+rcvf.file_name);

                if(upf.open(QIODevice::ReadWrite))
                {
                    upf.seek(0);
                    upf.write(rcvf.file_data);
                    upf.close();
                    recvstate = true;
                }
            }
            else
            {
                qDebug() << " update app file MD5 check err!!!";
                CCommonInterface::get_optbuzzer()->ringdidididi();
            }

            file.close();
            file.remove();
        }
    }

    if(recvstate)
    {
        switch(data->dataT)
        {
        case DATA_CARM:
            system("chmod 777 /"+rcvf.file_name);
            system("rm -f /opt/PDA/bin/tqpda; mv -f /"+rcvf.file_name+" /opt/PDA/bin/tqpda");
            system("fsync -d /opt/PDA/bin/tqpda; reboot");
            break;
        case DATA_C430:
            break;
        case DATA_CCFG:
            system("rm -f /BioTime/Option.ini /BioTime/back.ini");
            system("cp -f /" + rcvf.file_name + " " + CFG_STORE_PATH);
            system("mv -f /" + rcvf.file_name + " " + CFG_BACKUP_PATH);
            system("fsync -d /BioTime/Option.ini /BioTime/back.ini");
            CSysInfo::updt_cfg(CFG_STORE_PATH);
            CI::get_detection()->updateCardSlotVerify();
            qDebug() << "update Option.ini";
            break;
        default:
            break;
        }
    }
}

bool CDataCom::orgHistData(int ser, QByteArray *data)
{
    QSqlQuery query = CDataBase::select_Record(TABLE_BODY_NAME, "SERNUM", ser);

    if(query.next() && data != NULL)
    {
        qDebug() << "db ser =" << query.value(0).toInt() << ", ser =" << ser;

        data->append(intToByte(ser));//流水号
        data->append(query.value(1).toByteArray());//批号
        QDateTime dt = query.value(2).toDateTime();
        int st = QDateTime::fromString("1970-01-01  00:00:00","yyyy-MM-dd  hh:mm:ss").secsTo(dt);
        data->append(intToByte(st));//测试日期
        int unit = query.value(3).toInt();
        data->append(intToByte(unit));//联卡数量

        QByteArray temp;

        for(int i=0; i<6; i++)
        {
            temp.append(query.value(3 + 3 * i + 1).toByteArray());//项目名称

            while(temp.length() < (16 * (3 * i + 1)))
                temp.append(' ');

            temp.append(query.value(3 + 3 * i + 2).toByteArray());//单位

            while(temp.length() < (16 * (3 * i + 2)))
                temp.append(' ');

            temp.append(query.value(3 + 3 * i + 3).toByteArray());//结果

            while(temp.length() < (16 * (3 * i + 3)))
                temp.append(' ');
        }

        data->append(temp);
        return true;
    }

    return false;
}

void CDataCom::orgHistDatas(int from, int to)
{
    QSqlQuery query(tr("select * from " TABLE_BODY_NAME " where SERNUM>=%1 and SERNUM<=%2").arg(from).arg(to));
    if ( ! query.exec() )
    {
        qDebug() << query.lastError();
        return;
    }

    int heartBeat = 0;
    QByteArray * data;
    int st = 0;
    int unit = 0;
    QByteArray temp;
    SEND_ST tmpPkg;
    tmpPkg.comT = COM_RPT;
    tmpPkg.dataT = DATA_HIST;
    data = &tmpPkg.data;

    while(query.next())
    {
        data->clear();
        data->append(intToByte(query.value(0).toInt()));//流水号
        data->append(query.value(1).toByteArray());//批号
        QDateTime dt = query.value(2).toDateTime();
        st = QDateTime::fromString("1970-01-01  00:00:00","yyyy-MM-dd  hh:mm:ss").secsTo(dt);

        data->append(intToByte(st));//测试日期
        unit = query.value(3).toInt();
        data->append(intToByte(unit));//联卡数量

        temp.clear();
        for(int i=0; i<6; i++)
        {
            temp.append(query.value(3 + 3 * i + 1).toByteArray());//项目名称

            while(temp.length() < (16 * (3 * i + 1)))
                temp.append(' ');

            temp.append(query.value(3 + 3 * i + 2).toByteArray());//单位

            while(temp.length() < (16 * (3 * i + 2)))
                temp.append(' ');

            temp.append(query.value(3 + 3 * i + 3).toByteArray());//结果

            while(temp.length() < (16 * (3 * i + 3)))
                temp.append(' ');
        }

        data->append(temp);
        subPackageData(&tmpPkg);

        if (++heartBeat > 500)
        {
            heartBeat = 0;
            RecvHeartBeat();
        }
    }
}











