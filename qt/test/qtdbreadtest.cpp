
#include <QtTest>
#include <QObject>
#include "database.h"
#include "testpaths.h"

class DatabaseReadTest : public QObject {
    Q_OBJECT
    private Q_SLOTS:
        void testRead01();
};

using namespace Appstream;

void DatabaseReadTest::testRead01()
{
    // first, create a new database
    QProcess *p = new QProcess(this);
    QStringList args;
    QTemporaryDir dbdir;
    QVERIFY(dbdir.isValid());

    args.append("refresh-index");
    args.append("--datapath=" AS_SAMPLE_DATA);
    args.append("--dbpath=" + dbdir.path());
    p->start(ASCLI_EXECUTABLE, args);
    p->waitForFinished();

    qDebug() << p->readAllStandardOutput();
    qDebug() << p->readAllStandardError();

    QCOMPARE(p->exitCode(), 0);

    // we now have a database, let's read some data

    auto *db = new Database(dbdir.path() + "/xapian/default");
    QVERIFY(db->open());

    QList<Component> cpts = db->allComponents();
    QCOMPARE(cpts.size(), 18);

    Component cpt = db->componentById("neverball.desktop");
    QVERIFY(!cpt.id().isEmpty());

    QVERIFY(cpt.name() == "Neverball");

    delete db;
}

QTEST_MAIN(DatabaseReadTest)

#include "qtdbreadtest.moc"