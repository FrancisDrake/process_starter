#include <QString>
#include <QtTest>
#include "abstractprocess.h"
#include <stdexcept>
#include <QString>
#include <QHash>


class AbstractprocessTest : public QObject
{
    Q_OBJECT

public:
    AbstractprocessTest();

private Q_SLOTS:
    void worktest();
    void argumentsParseMultipleKey();
    void argumentsParseInvalidKey();
    void createProcess();
    void badConfigRead();
    void badPluginLoad();

};

AbstractprocessTest::AbstractprocessTest()
{
}

void AbstractprocessTest::worktest()
{
    // подстановка аргументов с дублированием одного из ключей
    const quint32 argc = 4;
    char* argv[argc] = {"testprocess", "-c../../tests/abstractprocesstest/testConfigComplexPlugin.xml", "-nChannel", "-sCoreServer"};
    const char* optstr = "n:s:c:";

    try
    {
        QHash<QString,QString> parameters = AbstractProcess::argumentsParse(optstr, argc, argv);
        AbstractProcess process(parameters);
    }
    catch(std::exception& a)
    {
        QVERIFY2(false,a.what());
    }
}


void AbstractprocessTest::argumentsParseMultipleKey()
{
    // подстановка аргументов с дублированием адного из ключей
    const quint32 argc = 4;
    char* argv[argc] = {"testprocess", "-cchannelprocessconfig.xml", "-nChannel", "-cCoreServer"};
    const char* optstr = "n:s:c:";

    try
    {
        QHash<QString,QString> parameters = AbstractProcess::argumentsParse(optstr, argc, argv);
    }
    catch(MultipleKeyException& a)
    {
        return;
    }
    QVERIFY2(false, "None exception with argv -cchannelprocessconfig.xml, -nChannel, -cCoreServer");
}

void AbstractprocessTest::argumentsParseInvalidKey()
{
    // подстановка аргументов когда один ключ не правильный
    quint32 argc = 4;
    char* argv[4] = {"testprocess", "-cchannelprocessconfig.xml", "-nChannel", "-dCoreServer"};
    const char* optstr = "n:s:c:";
    try
    {
        QHash<QString,QString> parameters = AbstractProcess::argumentsParse(optstr, argc, argv);
    }
    catch(InvalidKeyException& a)
    {
        return;
    }
    QVERIFY2(false, "None exception with invalid key parameter \"-cchannelprocessconfig.xml -nChannel -dCoreServer");
}

void AbstractprocessTest::createProcess()
{
    QHash<QString,QString> parameters;
    parameters.insert("m", "channelprocessconfig.xml");
    parameters.insert("s", "CoreServer.xml");
    parameters.insert("n", "Channel.xml");

    try
    {
        AbstractProcess process(parameters);
    }
    catch(std::invalid_argument& excp)
    {
        return;
    }
    QVERIFY2(false, "None exception with invalid key in parameters!");
}

void AbstractprocessTest::badConfigRead()
{
    QHash<QString,QString> parameters;
    parameters.insert("c", "../../tests/abstractprocesstest/testConfigComplexPlugi.xml");
    parameters.insert("s", "CoreServer.xml");
    parameters.insert("n", "Channel.xml");

    AbstractProcess process(parameters);
    try
    {
       // process.initProcess();
    }
    catch(ConfigException& error)
    {
        return;
    }
    QVERIFY2(false, "None exception with invalid config name");
}

void AbstractprocessTest::badPluginLoad()
{
    QHash<QString,QString> parameters;
    parameters.insert("c", "../../tests/abstractprocesstest/testConfigBadPlugin.xml");
    parameters.insert("s", "CoreServer.xml");
    parameters.insert("n", "Channel");

    AbstractProcess process(parameters);
    try
    {
       // process.initProcess();
    }
    catch(PluginLoadException& error)
    {
        return;
    }
    QVERIFY2(false, "None exception with failed plugin load!");
}

QTEST_APPLESS_MAIN(AbstractprocessTest)

#include "abstractprocesstest.moc"
