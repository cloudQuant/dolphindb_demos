#include "DolphinDB.h"
#include "Util.h"
#include "Types.h"
#include "ScalarImp.h" // 基本数据类型
#include "Set.h"
#include <iostream>
#include <vector>
using namespace dolphindb;

// 创建向量
VectorSP createDemoVector(){     //定义一个函数，后面会多次调用
    double* data = new double[10];
    for(int i = 0; i < 10; ++i){
        data[i] = i;
    }
    data[5] = getNullValue<double>(); // 设置index=5的时候为空
    return Util::createVector(DT_DOUBLE, 10, 10, true, 0, data, true);   //创建一个类型为DOUBLE，初始容量为10，初始大小为10的向量，并且使用自己分配的可以容纳10个double数据的内存，其中有空值
}

int main(){
    // 创建数据的两种方法
    ConstantSP i = new Int(1); //创建了一个Int类型的对象，值为1
    ConstantSP s = new String("DolphinDB"); //创建了一个String类型的对象，值为"DolphinDB"
    ConstantSP t = new Date(2022, 11, 1); //创建了一个Date类型对象，值为2022.11.1
    ConstantSP d = new Double(1.2); //创建了一个Double类型对象，值为1.2

    ConstantSP i1 = Util::createInt(1);
    ConstantSP s1 = Util::createString("DolphinDB");
    ConstantSP t1 = Util::createDate(2022, 11, 1);
    ConstantSP d1 = Util::createDouble(1.2);
    // 读取数据
    int ci = i->getInt();
    std::string cs = s->getString();
    int cd = t->getInt();     //Date底层数据类型是int
    double cv = d->getDouble();
    std::cout << "ci = " << ci << std::endl;
    std::cout << "cs = " << cs << std::endl;
    std::cout << "cd = " << cd << std::endl;
    std::cout << "cv = " << cv << std::endl;
    // 设置数据
    i->setInt(2);
    s->setString("C++ API");
    t->setInt(5000); //含义是距离1970年1月1日的天数
    d->setDouble(2.3);

    VectorSP vi = Util::createVector(DT_INT, 0, 100);                     //创建一个类型为INT，初始容量为100，初始大小为0的向量
    VectorSP vd = Util::createVector(DT_DECIMAL32, 0, 100, true, 5);          //创建一个类型为DECIMAL32(5),初始容量为100，初始大小为0的向量
    VectorSP vdb = createDemoVector();
    std::cout << vdb->getString() << std::endl;               //[0,1,2,3,4,,6,7,8,9]

    // 向量添加数据
    vi->append(new Int(1));           //[1]
    vi->append(new Int(4), 5);        //[1, 4, 4, 4, 4, 4]
    VectorSP vi2 = Util::createVector(DT_INT, 0, 100);        //[]
    vi2->append(vi, 2, 3);             //[4, 4, 4]

    VectorSP vi3 = Util::createVector(DT_INT, 0, 100);         //[]
    std::vector<int> v2{1, 2, 3, 4, 5};
    vi3->appendInt(v2.data(), v2.size());                     //[1, 2, 3, 4, 5]

    VectorSP vi4 = createDemoVector(); //[0,1,2,3,4,,6,7,8,9]
    ConstantSP e1 = vi4->get(1);     //e1为DT_DOUBLE类型的标量，值是1
    double e2 = vi4->getDouble(2);   //e2 = 2
    std::cout << "e2 = " << e2 << std::endl;

    // 批量将数据复制到指定的缓冲区中
    double buf[10]{};
    vi4->getDouble(0, 5, buf);        //buf: [0, 1, 2, 3, 4, 0, 0, 0, 0, 0]

    //获取只读的缓冲区，从而批量读取数据，方法声明如下：
    double buf2[10]{};
    const double* data = vi->getDoubleConst(0, 5, buf2);      //data: [0, 1, 2, 3, 4]
    std::cout << data << std::endl;

    // 更新数据
    // 按照下标来更新，方法声明如下
    VectorSP vi6 = createDemoVector();      //vi: [0,1,2,3,4,,6,7,8,9]
    vi6->set(1, new Double(100));       //vi: [0,100,2,3,4,,6,7,8,9]
    vi6->setDouble(2, 200);         //vi: [0,100,200,3,4,,6,7,8,9]
    // 批量更新
    double buf_[5];
    auto buf3 = vi->getDoubleBuffer(0, 5, buf_);
    for(int i = 0; i < 5; ++i){
        buf3[i] = buf3[i] * 2;         //将前五个元素的值乘以2
    }
    vi6->setDouble(0, 5, buf3);       //vi: [0,2,4,6,8,,6,7,8,9]

    // 创建矩阵及矩阵操作
    int* pData = new int[9]{1, 2, 3, 4, 5, 6, 7, 8, 9};
    ConstantSP matrix = Util::createMatrix(DT_INT, 3, 3, 9, 0, pData);
    std::cout << matrix->getString() << std::endl;

    std::cout << matrix->getRow(1)->getString() << std::endl;               //[2, 5, 8]
    std::cout << matrix->getColumn(1)->getString() << std::endl;            //[4, 5, 6]
    std::cout << matrix->get(2, 2)->getString() << std::endl;               //9
    matrix->set(0, 0, new Int(100));              //将第一行第一列的元素设置为100

    // 创建集合
    SetSP set1 = Util::createSet(DT_INT, 100);
    set1->append(new Int(3));      //（3）
    set1->append(new Int(1));             //(3, 1)
    set1->append(new Int(2));                      //(3, 1, 2)
    set1->append(new Int(2));                      //(3, 1, 2)
    ConstantSP result = new Bool;
    set1->contain(new Int(3), result);             //result : true
    set1->contain(new Int(4), result);             //result : false
    set1->remove(new Int(1));                      //(2, 3)
    set1->remove(new Int(4));                      //(2, 3)
    std::cout << set1->size();                     //2
    set1->clear();

    // 创建字典
    DictionarySP dic = Util::createDictionary(DT_INT, DT_STRING);
    dic->set(new Int(1), new String("123"));       //1 -> "123"
    dic->set(new Int(2), new String("456"));       //1 -> "123", 2 -> "456"
    dic->remove(new Int(2));                       //1 -> "123"
    dic->set(new Int(1), new String("777"));        //1 -> "777"

    ConstantSP result1 = new Bool;
    dic->contain(new Int(3), result1);              //result : false
    dic->contain(new Int(1), result1);              //result : true
    ConstantSP value = dic->getMember(new Int(1));
    std::cout << dic->size() << std::endl;         //1
    dic->clear();
    std::cout << dic->size() << std::endl;         //0
    std::cout << "字典操作成功" << std::endl;
    // 创建表
    std::vector<std::string> colNames{"col1", "col2", "col3"};
    std::vector<DATA_TYPE> colTypes{DT_INT, DT_BOOL, DT_STRING};
    TableSP tbl0 = Util::createTable(colNames, colTypes, 0, 100);

    std::vector<std::string> colNames1{"col1", "col2", "col3"};
    std::vector<ConstantSP> cols;
    cols.emplace_back(Util::createVector(DT_INT, 0));
    cols.emplace_back(Util::createVector(DT_BOOL, 0));
    cols.emplace_back(Util::createVector(DT_STRING, 0));
    TableSP tbl = Util::createTable(colNames1, cols);
    std::cout << "创建表成功" << std::endl;
    // 读取表数据
    // 读取列
    VectorSP col1 = tbl->getColumn(0);        // 获得第0列
    VectorSP col2 = tbl->getColumn("col2");   // 获取col2列
    std::cout << "读取列成功" << col1->getString() << std::endl;
    // 读取行,由于没有数据，读取失败
//    DictionarySP row = tbl->get(0);
//    std::cout << row->getMember("col1")->getInt() << std::endl;
//    std::cout << row->getMember("col3")->getString() << std::endl;
//    std::cout << "读取行列成功" << std::endl;
    // 通过ResultSet读取
    ResultSet resultSet(tbl);
    while (!resultSet.isAfterLast()) {
        int colIndex = 0;
        std::cout << resultSet.getInt(colIndex++) << " ";
        std::cout << int(resultSet.getBool(colIndex++)) << " ";
        std::cout << resultSet.getString(colIndex++) << std::endl;
        resultSet.next();
    }
    std::cout << "读取表数据成功" << std::endl;
    // 插入数据
    INDEX insertedRows;
    std::string errorMsg;
    VectorSP col0_ = Util::createVector(DT_INT, 0);
    VectorSP col1_ = Util::createVector(DT_BOOL, 0);
    VectorSP col2_ = Util::createVector(DT_STRING, 0);
    col0_->append(Util::createInt(1));
    col1_->append(Util::createBool(true));
    col2_->append(Util::createString("123"));
    std::vector<ConstantSP> values{col0_, col1_, col2_};
    tbl->append(values, insertedRows, errorMsg);  //values是由三个VectorSP组成，每个VectorSP对应表中的一列
    std::cout << "插入数据成功" << std::endl;
    TableSP tbl2 = tbl->getValue();
    std::cout << "tbl2 string = " << tbl2->getString() << std::endl;
    values= {tbl2};
    tbl->append(values, insertedRows, errorMsg); //values中只有一个元素，是另一个表
    TableSP tbl3 = tbl->getValue();
    std::cout << "tbl3 string = " << tbl3->getString() << std::endl;
    return 0;
}