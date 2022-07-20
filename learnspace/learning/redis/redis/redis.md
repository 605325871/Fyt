# 数据存储格式
* string
    * 存储的数据：单个数据，最简单最常用数据类型
    * 形式： 一个存储空间保存一个数据
    * 内容：通常字符串，若是数据以整数的形式展示，可以作为数字操作使用
  
>添加修改数据
    set key value
    获取数据
    get key
    删除数据
    del key

>添加修改多个数据
        mset key1 key2
    获取多个数据
        mget key1 key2
    获取字符串个数（字符长度）
    strlen key
    追加信息到原始信息后面（如果原始信息在就追加，否则新建）
    append key value

日常分表操作是基本操作，使用多张表存储同类型数据，但是对应的主键id必须保证主键id必须保证统一性，不能重复操作。orace数据库有sqeuence


string类型
* 设置数据增加指定范围的值
> incr key
>incrby key incrment
incrbyfloat key increment

* 设置数据减少指定范围的值
>decr key
decrby key increment

string在redis内部默认储存一个字符串，当遇到增减类操作incr decr的时候会转换成数值型进行计算
redis所有操作都是原子性，采用单线程处理所有业务，命令是一个一个执行的

* 设置数据的指定生命周期
> setex key seconds value
> psetex key milliseconds value

* 注意
> 1。表示运行结果是否成功数值要有所区分
> 2,数据未获取到nil等同于null
> 3.数据最大存储量为512mb

在redis中为热点信息存储，以用户