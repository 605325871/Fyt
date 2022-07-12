#include <iostream>
#include <string>
using namespace std;

class Exception
{
public:
    Exception(int errid, const char *errmsg)
        : _errid(errid), _errmsg(errmsg)
    {
    }
    virtual string what() const
    {
        return _errmsg;
    }

protected:
    string _errmsg;
    int _errid;
};

class SqlExceptin : public Exception
{

public:
    SqlExceptin(int errid, const char *errmsg, const char *sql = "")
        : Exception{errid, errmsg}, _sql(sql)
    {
    }
    virtual string what() const
    {
        string msg = "sqlException";
        msg += _errmsg;
        msg += "sql";
        msg += _sql;
        return msg;
    }
    ~SqlExceptin() {}

private:
    string _sql;
};
class CacheException : public Exception
{
public:
    CacheException(int errid, const char *errmsg)
        : Exception(errid, errmsg)
    {
    }
    virtual string what() const
    {
        string msg = "CacheException:";
        msg += _errmsg;
        return msg;
    }
};

void f1()
{
    int i;
    cin >> i;
    if (i == 1)
        throw CacheException(1, "数据不存在");
}
void f2()
{
    int i;
    cin >> i;
    throw SqlExceptin(1, "数据库查询失败", "select * from t_student");
}
int main()
{
    try
    {
        f1();
        f2();
    }
    catch (const Exception &e) //多态
    {
        cout << e.what() << endl;
    }
    catch (...)
    {
        cout << "unkown exception" << endl;
    }
}