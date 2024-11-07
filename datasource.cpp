#include "datasource.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

DataSource::DataSource() {}

TreeItem *DataSource::countryForRowColumn(int row, int column)
{
    if(column!=0)
        return nullptr;

    if(row < countries.size())
    {
        return countries.at(row);
    }

    return nullptr;
}

TreeItem *DataSource::operForRowColumn(int row, int column, int countryRow)
{
    if(column!=0)
        return nullptr;

    TreeItem* country=countryForRowColumn(countryRow, 0);

    if(country && country->children.size()>row)
    {
        return country->children.at(row);
    }

    return nullptr;
}

int DataSource::rowForCountry(qint16 code)
{
    auto it=std::find_if(countries.begin(), countries.end(),
                           [=](TreeItem* elem){
                               return elem->code()==code;
    });

    if(it!=countries.end())
        return it-countries.begin();

    return -1;
}

int DataSource::countCountries()
{
    return countries.size();
}

int TreeItem::countChildren()
{
    return children.size();
}

const QImage &TreeItem::decoration()
{
    return decoration_;
}

const QString Oper::pattern("%1 (%2-%3)");

qint16 Oper::code()
{
    return code_;
}

qint16 Oper::countryCode()
{
    return country_code;
}

QString Oper::displayName()
{
    return pattern.arg(name).arg(country_code).arg(code_);
}

const QString Country::pattern("%1 (%2)");

qint16 Country::code()
{
    return code_;
}

qint16 Country::countryCode()
{
    return -1;
}

QString Country::displayName()
{
    return pattern.arg(name, iso2);
}

SqlDataSource::SqlDataSource(const QString &fileName):
    dbFileName(fileName)
{}

SqlDataSource::~SqlDataSource()
{

}

bool SqlDataSource::aquireData()
{
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbFileName);

    if(!db.open())
    {
        return false;
    }

    QSqlQuery query("SELECT countries.mcc as mcc, countries.code as country_code, countries.mnc_length as mnc_length,"
                    "countries.name as country_name, operators.mnc as mnc, operators.name as operator_name "
                    "FROM countries INNER join operators on countries.mcc=operators.mcc "
                    "ORDER by country_code");

    if (query.exec())
    {
        Country* cElement=nullptr;

        static const QString c_icon_pattern(":/icons/Countries/%1.png");
        static const QString o_icon_pattern(":/icons/Operators/%1_%2.png");

        while (query.next()) {
            qint16 mcc=query.value("mcc").toInt();
            if(!(cElement && cElement->code_==mcc))
            {
                cElement=new Country;
                cElement->parent=nullptr;
                cElement->code_=mcc;
                cElement->iso2=query.value("country_code").toString();
                cElement->name=query.value("country_name").toString();
                cElement->decoration_=QImage(c_icon_pattern.arg(cElement->iso2));

                countries.push_back(cElement);
            }

            Oper* oElement=new Oper;
            oElement->parent=cElement;
            oElement->code_=query.value("mnc").toInt();
            oElement->country_code=cElement->code_;
            oElement->name=query.value("operator_name").toString();
            oElement->decoration_=QImage(o_icon_pattern.arg(cElement->code_).arg(oElement->code_));

            cElement->children.push_back(oElement);
        }
    }
    else
        return false;

    return true;
}
