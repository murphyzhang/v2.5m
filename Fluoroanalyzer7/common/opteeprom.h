#ifndef OPTEEPROM_H
#define OPTEEPROM_H

#include <QThread>

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

#include <unistd.h>
#include <stdlib.h>
#include <linux/fs.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#include <assert.h>
#include <string.h>

#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include "xioctl.h"

#define CHIP_ADDR	0x50		//设备地址
#define I2C_DEV		"/dev/i2c-0"

#define CHECK_IO "g3"

#include "argdecode.h"
#include "DataBase.h"

struct eeprom
{
        char *dev; 			// 设备名称，i2c-0
        int addr;			// 设备中地址
        int fd;				// 设备句柄
};


class COptEeprom : public QThread
{
    Q_OBJECT
public:
    typedef enum
    {
        CHECK_RUN,
        READ_RUN,
        WRITE_RUN,
        WAIT_RUN
    }RUNTYPE;

    explicit COptEeprom(QObject *parent = 0);
    int init_eeprom();
    void run();
    bool eeprom_insert();
    inline __s32 i2c_smbus_access(int file, char read_write, __u8 command, int size, union i2c_smbus_data *data);
    int eeprom_read_byte(struct eeprom* e, __u16 mem_addr);
    int eeprom_write_byte(struct eeprom *e, __u16 mem_addr, __u8 data);
    int eeprom_open(char *dev_name, int addr, struct eeprom *e);
    int eeprom_close(struct eeprom *e);
    int eeprom_read_n(struct eeprom *e, int addr, int size, QByteArray *buf);
    int eeprom_write_n(struct eeprom *e, int addr, int size, QByteArray buf);

    int read_from_eeprom(int addr, int size, QByteArray *buf);
    int write_to_eeprom( int addr, int size, QByteArray buf);
signals:
    //void sigreadnewcode();
    void SigNewBatch(QString batch);
public slots:

private:
    struct eeprom m_eep;
    RUNTYPE m_runtype;
    QByteArray m_code;
    bool m_isnewcard;

};

#endif // OPTEEPROM_H
