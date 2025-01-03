#include "ccapi_cpp/ccapi_session.h"
#include "DolphinDB.h"
#include "MultithreadedTableWriter.h"
#include "Util.h"
#include "Types.h"
#include "ScalarImp.h" // 基本数据类型
#include "Set.h"
#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <chrono>
#include <memory>
#include <iomanip>
#include <ctime>
#include <sstream>  // 添加这个头文件
using namespace dolphindb;
using namespace std;
using namespace std::chrono;
using std::this_thread::sleep_for;

namespace ccapi {
Logger* Logger::logger = nullptr;  // This line is needed.

class MyEventHandler : public EventHandler {
 public:
  std::vector<std::string> instrument_names;
  //构造MTW对象
  ErrorCodeInfo errorInfo;
  long long count;
  long num;
  // 声明成员变量
  std::unique_ptr<MultithreadedTableWriter> mark_price_writer;
  std::unique_ptr<MultithreadedTableWriter> funding_rate_writer;
  std::unique_ptr<MultithreadedTableWriter> orderbook_writer;
  std::unique_ptr<MultithreadedTableWriter> force_order_writer;
  std::unique_ptr<MultithreadedTableWriter> agg_trade_writer;


  MyEventHandler(const std::string &user_id, const std::string &password) {
    try {
        // 初始化成员变量
        mark_price_writer = std::make_unique<MultithreadedTableWriter>("127.0.0.1", 8848, user_id, password, "dfs://binance_data", "mark_price", false, false, nullptr, 10000, 30, 1);
        funding_rate_writer = std::make_unique<MultithreadedTableWriter>("127.0.0.1", 8848, user_id, password, "dfs://binance_data", "funding_rate", false, false, nullptr, 10000, 30, 1);
        orderbook_writer = std::make_unique<MultithreadedTableWriter>("127.0.0.1", 8848, user_id, password, "dfs://binance_data", "orderbook", false, false, nullptr, 10000, 30, 1);
        force_order_writer = std::make_unique<MultithreadedTableWriter>("127.0.0.1", 8848, user_id, password, "dfs://binance_data", "force_order", false, false, nullptr, 10000, 30, 1);
        agg_trade_writer = std::make_unique<MultithreadedTableWriter>("127.0.0.1", 8848, user_id, password, "dfs://binance_data", "agg_trade", false, false, nullptr, 10000, 30, 1);
    } catch (const std::exception& e) {
        // Handle the exception, log it, or rethrow it if necessary
        std::cerr << "Error initializing writers: " << e.what() << std::endl;
        // Optionally rethrow the exception
        // throw;
    }
    // Initialize other members
    count = 0;
    num = 0;
  }

  void update_num(){
    num++;
    if (num%1000000 == 0){
      count++;
      num = 0;
      std::cout << "recieve data num " << count << "million" << std::endl;
    }
  }

  void save_binance_depth_data(Message & msg){
    std::string symbol_id = UtilString::toUpper(msg.getSymbolId());
    long long server_timestamp = UtilTime::timePointToTimestamp(msg.getTime());
    long long local_timestamp = UtilTime::timePointToTimestamp(msg.getTimeReceived());
    std::vector<Element> elements = msg.getElementList();
    std::vector<double> ask_price_list;
    std::vector<double> bid_price_list;
    std::vector<double> ask_volume_list;
    std::vector<double> bid_volume_list;
    for (auto & element : elements){
      if (element.has("BID_PRICE")){
        bid_price_list.push_back(std::stod(element.getValue("BID_PRICE")));
        bid_volume_list.push_back(std::stod(element.getValue("BID_SIZE")));
      }
      if (element.has("ASK_PRICE")){
        ask_price_list.push_back(std::stod(element.getValue("ASK_PRICE")));
        ask_volume_list.push_back(std::stod(element.getValue("ASK_SIZE")));
      }
    }
    ConstantSP datetime = new Long(server_timestamp/1000000000);
    ConstantSP server_time = new Long(server_timestamp/1000000);
    ConstantSP local_update_time = new Double(local_timestamp/1000000.0);
    ConstantSP symbol = new String(symbol_id);
      
    if(!orderbook_writer->insert(errorInfo, datetime, server_time, local_update_time, symbol, ask_price_list, bid_price_list,
        ask_volume_list, bid_volume_list)){
        std::cout << "order_book_insert fail " << errorInfo.errorInfo << std::endl;
    }
    update_num();
    //std::cout <<"depth_data " <<  symbol_id << " " << msg.getTime() << " " << local_timestamp << std::endl;
  }

  void save_binance_agg_trade_data(Message & msg){
    std::string symbol_id = UtilString::toUpper(msg.getSymbolId());
    long long server_timestamp = UtilTime::timePointToTimestamp(msg.getTime());
    long long local_timestamp = UtilTime::timePointToTimestamp(msg.getTimeReceived());
    std::vector<Element> elements = msg.getElementList();
    for (auto element : elements){
      std::string _trade_id = element.getValue("AGG_TRADE_ID");
      std::string _trade_first_id = element.getValue("AGG_TRADE_FIRST_ID");
      std::string _trade_last_id = element.getValue("AGG_TRADE_LAST_ID");
      std::string _trade_type = element.getValue("IS_BUYER_MAKER");
      double _trade_price = std::stod(element.getValue("LAST_PRICE"));
      double _trade_qty = std::stod(element.getValue("LAST_SIZE"));
      ConstantSP datetime = new Long(server_timestamp/1000000000);
      ConstantSP server_time = new Long(server_timestamp/1000000);
      ConstantSP local_update_time = new Double(local_timestamp/1000000.0);
      ConstantSP symbol = new String(symbol_id);
      ConstantSP trade_id = new String(_trade_id);
      ConstantSP trade_first_id = new String(_trade_first_id);
      ConstantSP trade_last_id = new String(_trade_last_id);
      ConstantSP trade_type = new String(_trade_type);
      ConstantSP trade_price = new Double(_trade_price);
      ConstantSP trade_qty = new Double(_trade_qty);
      ConstantSP trade_time = new Double(server_timestamp);
      if(!agg_trade_writer->insert(errorInfo, datetime, server_time, local_update_time, symbol, trade_id, trade_first_id, trade_last_id,
          trade_type, trade_price, trade_qty, trade_time)){
          std::cout << "agg_trade_insert fail " << errorInfo.errorInfo << std::endl;
      }
      update_num();
    }
    // std::cout << toString(msg) << std::endl;
    //std::cout <<"agg_trade " <<  symbol_id << " " << " " << msg.getTime() << " " << local_timestamp << std::endl;
  };

  void save_binance_force_order_data(Message & msg){
    //std::string server_time_ = msg.getTimeISO();
    //std::string local_update_time_ = msg.getTimeReceivedISO();
    long long server_timestamp = UtilTime::timePointToTimestamp(msg.getTime());
    long long local_timestamp = UtilTime::timePointToTimestamp(msg.getTimeReceived());
    std::vector<Element> elements = msg.getElementList();
    // std::cout << toString(msg) << std::endl;
    for (auto element : elements){
      std::string symbol_id = UtilString::toUpper(element.getValue("symbol_id"));
      std::string _order_side = element.getValue("order_side");
      std::string _order_type = element.getValue("order_type");
      std::string _order_time_in_force = element.getValue("order_force");
      std::string _trade_status = element.getValue("order_status");
      double _order_price = std::stod(element.getValue("order_price"));
      double _order_qty = std::stod(element.getValue("order_qty"));
      double _order_avg_price = std::stod(element.getValue("order_avg_price"));
      double _last_trade_qty = std::stod(element.getValue("order_last_trade_qty"));
      double _order_cumsum_trade_qty = std::stod(element.getValue("order_cumsum_trade_qty"));
      ConstantSP datetime = new Long(server_timestamp/1000000000);
      ConstantSP server_time = new Long(server_timestamp/1000000);
      ConstantSP local_update_time = new Double(local_timestamp/1000000.0);
      ConstantSP symbol = new String(symbol_id);
      ConstantSP order_side = new String(_order_side);
      ConstantSP order_type = new String(_order_type);
      ConstantSP order_time_in_force = new String(_order_time_in_force);
      ConstantSP trade_status = new String(_trade_status);
      ConstantSP order_price = new Double(_order_price);
      ConstantSP order_qty = new Double(_order_qty);
      ConstantSP order_avg_price = new Double(_order_avg_price);
      ConstantSP last_trade_qty = new Double(_last_trade_qty);
      ConstantSP order_cumsum_trade_qty = new Double(_order_cumsum_trade_qty);
      if(!force_order_writer->insert(errorInfo, datetime, server_time, local_update_time, symbol, order_side,order_type,_order_time_in_force,
          trade_status, order_price, order_qty, order_avg_price, server_time, last_trade_qty, order_cumsum_trade_qty)){
          std::cout << "force_order_insert fail " << errorInfo.errorInfo << std::endl;
      }
      update_num();
      //std::cout <<"force_order " <<  symbol_id << " " << msg.getTime() << " " << local_timestamp << std::endl;
    }
  }

  void save_binance_mark_price_data(Message & msg){
    long long server_timestamp = UtilTime::timePointToTimestamp(msg.getTime());
    long long local_timestamp = UtilTime::timePointToTimestamp(msg.getTimeReceived());
    std::vector<Element> elements = msg.getElementList();
    for (auto element : elements){
        //std::string _server_time = element.getValue("server_time");
      std::string symbol_id = UtilString::toUpper(element.getValue("symbol_id"));
      double _funding_rate = std::stod(element.getValue("funding_rate"));
      double _mark_price = std::stod(element.getValue("mark_price"));
      double _next_funding_rate_time = std::stod(element.getValue("next_funding_rate_time"));
      double _predicted_settlement_price = std::stod(element.getValue("predicted_settlement_price"));
      double _spot_index_price = std::stod(element.getValue("spot_index_price"));
      ConstantSP datetime = new Long(server_timestamp/1000000000);
      ConstantSP server_time = new Long(server_timestamp/1000000);
      ConstantSP local_update_time = new Double(local_timestamp/1000000.0);
      ConstantSP symbol = new String(symbol_id);
      ConstantSP mark_price = new Double(_mark_price);
      ConstantSP index_price = new Double(_spot_index_price);
      ConstantSP settlement_price = new Double(_predicted_settlement_price);

      if(!mark_price_writer->insert(errorInfo, datetime, server_time, local_update_time, symbol, mark_price, index_price, settlement_price)){
          std::cout << "mark_price_insert fail " << errorInfo.errorInfo << std::endl;
      }
      ConstantSP current_funding_rate = new Double(_funding_rate);
      ConstantSP next_funding_rate = new Double(0);
      ConstantSP next_funding_rate_time = new Double(_next_funding_rate_time);
      if(!funding_rate_writer->insert(errorInfo, datetime, server_time, local_update_time, symbol, current_funding_rate, next_funding_rate, next_funding_rate_time)){
          std::cout << "funding_rate insert fail " << errorInfo.errorInfo << std::endl;
      }
      update_num();
      //std::cout <<"mark_price " <<  symbol_id << " "<<_server_time << " " << server_timestamp << " " << local_update_time_ << std::endl;
    }
  }
  void deal_with_binance_instruments(Message & msg){
    std::vector<Element> elements = msg.getElementList();
    for (auto & element : elements){
      if (element.has("INSTRUMENT")){
        instrument_names.push_back(UtilString::toLower(element.getValue("INSTRUMENT")));
      }
    }
  }
  bool processEvent(const Event& event, Session* session) override {
    // std::cout << toString(event) + "\n" << std::endl;
    std::vector<Message> msgs = event.getMessageList();
    for (auto msg : msgs){
        // 如果是深度数据
        if (msg.getType() == Message::Type::MARKET_DATA_EVENTS_MARKET_DEPTH){
          save_binance_depth_data(msg);
        }
        // 如果是归集交易(agg_trade)
        if (msg.getType() == Message::Type::MARKET_DATA_EVENTS_AGG_TRADE){
          save_binance_agg_trade_data(msg);
        }
        // 如果是标记价格数据
        if (msg.getType() == Message::Type::MARKET_DATA_EVENTS_MARK_PRICE){
          save_binance_mark_price_data(msg);
        }
        // 如果是强平订单数据
        if (msg.getType() == Message::Type::MARKET_DATA_EVENTS_FORCE_ORDER){
          save_binance_force_order_data(msg);
        }
        // request获取合约数据
        if (msg.getType() == Message::Type::GET_INSTRUMENTS){
          deal_with_binance_instruments(msg);
        }
    }

    
    return true;
  }
};
} /* namespace ccapi */
using ::ccapi::Event;
using ::ccapi::EventDispatcher;
using ::ccapi::MyEventHandler;
using ::ccapi::Session;
using ::ccapi::Request;
using ::ccapi::SessionConfigs;
using ::ccapi::SessionOptions;
using ::ccapi::Subscription;
using ::ccapi::toString;
int main(int argc, char** argv) {
    const std::string user_id = "admin";
    const std::string password = "123456";
  DBConnection conn;
  bool ret = conn.connect("localhost", 8848, user_id, password);
  if (!ret) {
    std::cout << "Failed to connect to the server" << std::endl;
    return 0;
  }else{
      ConstantSP result= conn.run("1+1");
      std::cout << result->getString() << std::endl;
  }

  // std::cout << "CCAPI_EXCHANGE_NAME_BINANCE_USDS_FUTURES = " << CCAPI_EXCHANGE_NAME_BINANCE_USDS_FUTURES << std::endl;
  std::vector<std::string> instrument_names;
  std::vector<std::string> modeList = {
      "dispatch_events_to_multiple_threads",
      "handle_events_in_batching_mode",
  };
  if (argc != 2 || std::find(modeList.begin(), modeList.end(), argv[1]) == modeList.end()) {
    std::cerr << "Please provide one command line argument from this list: " + toString(modeList) << std::endl;
    return EXIT_FAILURE;
  }
  std::string mode(argv[1]);
  if (mode == "dispatch_events_to_multiple_threads") {
    SessionOptions sessionOptions;
    SessionConfigs sessionConfigs;
    MyEventHandler eventHandler{user_id, password};
    EventDispatcher eventDispatcher(5);
    Session session(sessionOptions, sessionConfigs, &eventHandler, &eventDispatcher);
    // 订阅全部标记价格
    Subscription s0("binance-usds-futures", "", "MARK_PRICE", "", "mark_price");
    // 订阅全部强平订单
    Subscription s1("binance-usds-futures", "", "FORCE_ORDER", "", "force_order");
    // 生成标记价格和强平订单的订阅
    std::vector<Subscription> subscription_list = {s0, s1};
    // std::vector<Subscription> subscription_list = {s0};
    // 请求全部合约数据
    Request request(Request::Operation::GET_INSTRUMENTS, "binance-usds-futures", "BTC-USD");
    session.sendRequest(request);
    while (eventHandler.instrument_names.size()==0){
      std::this_thread::sleep_for(std::chrono::seconds(1));
      std::cout << "wait for the instruments_names" << std::endl;
    }
    // 生成其他的订阅
    int count = 0;
    for (auto name : eventHandler.instrument_names){
      Subscription s2("binance-usds-futures", name, "MARKET_DEPTH", "MARKET_DEPTH_MAX=20", "depth");
      Subscription s3("binance-usds-futures", name, "AGG_TRADE", "", "agg_trade");
      subscription_list.push_back(s2);
      subscription_list.push_back(s3);
      count++;
      if (count > 20){
        std::this_thread::sleep_for(std::chrono::seconds(1));
        session.subscribe(subscription_list);
        subscription_list.clear();
        count = 0;
        break;
        }
    }
    // Subscription s3("binance-usds-futures", "btcusdt", "AGG_TRADE", "", "agg_trade");
    // Subscription s4("binance-usds-futures", "btcusdt", "MARKET_DEPTH", "MARKET_DEPTH_MAX=20", "depth");
    // std::vector<Subscription> subscription_list = {s4};
    std::cout << "begin to subscribe data" << std::endl;
    session.subscribe(subscription_list);
    // 无限循环以保持程序运行
    while (true) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    session.stop();
    eventDispatcher.stop();
  } else if (mode == "handle_events_in_batching_mode") {
    SessionOptions sessionOptions;
    SessionConfigs sessionConfigs;
    Session session(sessionOptions, sessionConfigs);
    Subscription subscription("binance-usds-futures", "ethusdt", "MARKET_DEPTH", "", "binance-us__ethusd");
    session.subscribe(subscription);
    // 无限循环以保持程序运行
    while (true) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::vector<Event> eventList = session.getEventQueue().purge();
    for (const auto& event : eventList) {
      std::cout << toString(event) + "\n" << std::endl;
    }
    session.stop();
  }
  std::cout << "Bye" << std::endl;
  return EXIT_SUCCESS;
}
