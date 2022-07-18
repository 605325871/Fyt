#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
using namespace std;

int main()
{
    int i = 1;
    int j = 2;
    bool flg = true;
    condition_variable cv;
    mutex mtx;
    thread t2([&j, &mtx, &cv,&flg]()
              {
        while(j<=100)
        {
            std:: unique_lock<mutex> lock(mtx);

            cv.wait(lock,[&flg](){return flg;});
            cout<<std::this_thread::get_id()<<":"<<j<<endl;
            j+=2;
            flg=false; 
            cv.notify_one();
        } });
    thread t1([&i, &mtx, &cv,&flg]()
              {
        while(i<=100)
        {
            std:: unique_lock<mutex> lock(mtx);
            cv.wait(lock,[&flg](){return !flg;});
            cout<<std::this_thread::get_id()<<":"<<i<<endl;
            i+=2;
            flg=true;
             cv.notify_one();
        } });
    t1.join();
    t2.join();
}