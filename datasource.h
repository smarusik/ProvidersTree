#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <deque>
#include <QString>
#include <QImage>

struct TreeItem
{
    TreeItem *parent;
    std::deque<TreeItem*> children;
    QImage decoration_;

    int countChildren();
    virtual qint16 code()=0;
    virtual qint16 countryCode()=0;
    virtual QString displayName()=0;
    virtual const QImage& decoration();
};

struct Oper: public TreeItem
{
    static const QString pattern;

    quint16 code_;
    QString name;
    quint16 country_code;

    // TreeItem interface
public:
    qint16 code() override;

    qint16 countryCode() override;

    QString displayName() override;
};

struct Country: public TreeItem
{
    static const QString pattern;
    quint16 code_;
    QString iso2;
    QString name;

    bool operator <(const Country& other);

    // TreeItem interface
public:
    qint16 code() override;

    qint16 countryCode() override;

    QString displayName() override;
};

class DataSource
{
protected:
    std::deque<TreeItem*> countries;

public:
    DataSource();

    virtual bool aquireData()=0;
    TreeItem *countryForRowColumn(int row, int column);
    TreeItem* operForRowColumn(int row, int column, int countryRow);
    int rowForCountry(qint16 code);
    int countCountries();

};

class SqlDataSource: public DataSource
{
    QString dbFileName;
public:
    SqlDataSource(const QString &fileName);
    ~SqlDataSource();

    bool aquireData() override;
};

#endif // DATASOURCE_H
