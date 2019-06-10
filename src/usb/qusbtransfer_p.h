#ifndef QUSBTRANSFER_P_H
#define QUSBTRANSFER_P_H

#include "qusbtransfer.h"
#include <private/qiodevice_p.h>
#include <QMutexLocker>
#include <QReadWriteLock>
#include <QQueue>

#ifdef Q_OS_UNIX
#include <libusb.h>
#else
#include <libusb/libusb.h>
#endif

QT_BEGIN_NAMESPACE

class BuffQueue{
public :
    BuffQueue(){init();}
private :
    QQueue<QByteArray *> buffs;
    QReadWriteLock mutex;
    qint64 buffLen;
public :
    void init(){
        release();
    }

    qint64 bytesAvailable()
    {
//        QReadLocker(&mutex);
        mutex.lockForWrite();
        qint64 len = buffLen;
        mutex.unlock();
        return len;
    }

    void put(QByteArray *buff){
        if(buff == NULL)
            return;
        mutex.lockForWrite();
        buffLen += buff->length();
        buffs.push_back(buff);
        mutex.unlock();
    }

    QByteArray *get(){
        QByteArray *buff = NULL;
        mutex.lockForWrite();
        if(buffs.size() > 0){
            buff = buffs.front();
            buffs.pop_front();
        }
        if(buff)
            buffLen -= buff->length();
        mutex.unlock();
        return buff;
    }

    int size(){
        mutex.lockForRead();
        int len = buffs.size();
        mutex.unlock();
        return len;
    }

    void release(){
        mutex.lockForWrite();
        QQueue<QByteArray *>::iterator begin = buffs.begin();
        QQueue<QByteArray *>::iterator end = buffs.end();
        while(begin != end){
            QByteArray *buff = *begin;
            if(buff != NULL){
                buff->clear();
                delete buff;
            }
            buffs.pop_front();
            begin = buffs.begin();
        }
        buffLen = 0;
        mutex.unlock();
    }
};

class QUsbTransferPrivate : public QIODevicePrivate
{
    Q_DECLARE_PUBLIC(QUsbTransfer)

public:
    QUsbTransferPrivate();
    ~QUsbTransferPrivate();

    void readyRead();
    void bytesWritten(qint64 bytes);
    void error(QUsbTransfer::Status error);
    void setStatus(QUsbTransfer::Status status);
    bool isValid();

    bool prepareTransfer(libusb_transfer **tr, libusb_transfer_cb_fn cb, char *data, qint64 size, QUsbDevice::Endpoint ep);
    void stopTransfer();

    int readUsb(qint64 maxSize);
    int writeUsb(const char *data, qint64 maxSize);

    void setPolling(bool enable);
    bool polling() { return m_poll; }

    QUsbDevice::LogLevel logLevel();

    bool m_poll;
    int m_poll_size;

    libusb_transfer *m_transfer_in, *m_transfer_out;
    QByteArray m_write_buf, /*m_read_buf,*/ m_read_transfer_buf;
    QMutex m_read_transfer_mutex,  m_write_buf_mutex;


    BuffQueue* _buffQurue = nullptr;
    qint64 _readLen = 0;
    QReadWriteLock m_read_buf_mutex;
};

QT_END_NAMESPACE

#endif
