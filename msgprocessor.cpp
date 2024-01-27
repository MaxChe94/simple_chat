#include "msgprocessor.h"

MsgProcessor::MsgProcessor(QObject *parent) : QObject(parent)
{

}

MsgProcessor::MsgProcessor(QString name)
{
    this->name = name;
}

