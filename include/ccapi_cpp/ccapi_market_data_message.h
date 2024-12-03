#ifndef INCLUDE_CCAPI_CPP_CCAPI_MARKET_DATA_MESSAGE_H_
#define INCLUDE_CCAPI_CPP_CCAPI_MARKET_DATA_MESSAGE_H_

#include "ccapi_cpp/ccapi_logger.h"
#include "ccapi_cpp/ccapi_util_private.h"
// #include "ccapi_message.h"
namespace ccapi {
class MarketDataMessage CCAPI_FINAL {
  /**
   * A handle to a single market data message. Each MarketDataMessage is associated with one 'exchangeSubscriptionId' value. The MarketDataMessage contents are
   * represented in its 'data' attribute. Each MarketDataMessage object consists of an Type attribute and a RecapType attribute. The exchange timestamp (if any)
   * associated with the MarketDataMessage object is represented in its 'tp' attribute.
   */
 public:
  enum class Type {
    UNKNOWN,
    MARKET_DATA_EVENTS_MARKET_DEPTH,
    MARKET_DATA_EVENTS_TRADE,
    MARKET_DATA_EVENTS_AGG_TRADE,
    MARKET_DATA_EVENTS_CANDLESTICK,
    MARKET_DATA_EVENTS_MARK_PRICE,
    MARKET_DATA_EVENTS_FORCE_ORDER,
  };
  enum class RecapType {
    UNKNOWN,
    NONE,       // normal data tick; not a recap
    SOLICITED,  // generated on request by subscriber
  };
  static std::string recapTypeToString(RecapType recapType) {
    std::string output;
    switch (recapType) {
      case RecapType::UNKNOWN:
        output = "UNKNOWN";
        break;
      case RecapType::NONE:
        output = "NONE";
        break;
      case RecapType::SOLICITED:
        output = "SOLICITED";
        break;
      default:
        CCAPI_LOGGER_FATAL(CCAPI_UNSUPPORTED_VALUE);
    }
    return output;
  }
  enum class DataType {
    BID = 0,
    ASK = 1,
    TRADE = 2,
    AGG_TRADE = 3,
    CANDLESTICK = 4,
    MARK_PRICE = 5,
    FORCE_ORDER = 6,
  };
  static std::string dataTypeToString(DataType dataType) {
    std::string output;
    switch (dataType) {
      case DataType::BID:
        output = "BID";
        break;
      case DataType::ASK:
        output = "ASK";
        break;
      case DataType::TRADE:
        output = "TRADE";
        break;
      case DataType::AGG_TRADE:
        output = "AGG_TRADE";
        break;
      case DataType::CANDLESTICK:
        output = "CANDLESTICK";
        break;
      case DataType::MARK_PRICE:
        output = "MARK_PRICE";
        break;
      case DataType::FORCE_ORDER:
        output = "FORCE_ORDER";
        break;
      default:
        CCAPI_LOGGER_FATAL(CCAPI_UNSUPPORTED_VALUE);
    }
    return output;
  }
  enum class DataFieldType {
    PRICE = 0,
    SIZE = 1,
    TRADE_ID = 2,
    AGG_TRADE_ID = 3,
    IS_BUYER_MAKER = 4,
    SEQUENCE_NUMBER = 5,
    OPEN_PRICE = 6,
    HIGH_PRICE = 7,
    LOW_PRICE = 8,
    CLOSE_PRICE = 9,
    VOLUME = 10,
    QUOTE_VOLUME = 11,
    AGG_TRADE_FIRST_ID = 12,
    AGG_TRADE_LAST_ID = 13,
    MARK_PRICE = 14,
    SPOT_INDEX_PRICE = 15,
    PREDICTED_SETTLEMENT_PRICE = 16,
    FUNDING_RATE = 17,
    NEXT_FUNDING_RATE_TIME = 18,
    ORDER_SIDE = 19,
    ORDER_TYPE = 20,
    ORDER_FORCE = 21,
    ORDER_QTY = 22,
    ORDER_PRICE = 23,
    ORDER_AVG_PRICE = 24,
    ORDER_STATUS = 25,
    ORDER_LAST_TRADE_QTY = 26,
    ORDER_CUMSUM_TRADE_QTY = 27,
  };
  static std::string dataFieldTypeToString(DataFieldType dataFieldType) {
    std::string output;
    switch (dataFieldType) {
      case DataFieldType::PRICE:
        output = "PRICE";
        break;
      case DataFieldType::SIZE:
        output = "SIZE";
        break;
      case DataFieldType::TRADE_ID:
        output = "TRADE_ID";
        break;
      case DataFieldType::AGG_TRADE_ID:
        output = "AGG_TRADE_ID";
        break;
      case DataFieldType::IS_BUYER_MAKER:
        output = "IS_BUYER_MAKER";
        break;
      case DataFieldType::SEQUENCE_NUMBER:
        output = "SEQUENCE_NUMBER";
        break;
      case DataFieldType::OPEN_PRICE:
        output = "OPEN_PRICE";
        break;
      case DataFieldType::HIGH_PRICE:
        output = "HIGH_PRICE";
        break;
      case DataFieldType::LOW_PRICE:
        output = "LOW_PRICE";
        break;
      case DataFieldType::CLOSE_PRICE:
        output = "CLOSE_PRICE";
        break;
      case DataFieldType::VOLUME:
        output = "VOLUME";
        break;
      case DataFieldType::QUOTE_VOLUME:
        output = "QUOTE_VOLUME";
        break;
      case DataFieldType::AGG_TRADE_FIRST_ID:
        output = "AGG_TRADE_FIRST_ID";
        break;
      case DataFieldType::AGG_TRADE_LAST_ID:
        output = "AGG_TRADE_LAST_ID";
        break;
      case DataFieldType::MARK_PRICE:
        output = "MARK_PRICE";
        break;
      case DataFieldType::SPOT_INDEX_PRICE:
        output = "SPOT_INDEX_PRICE";
        break;
      case DataFieldType::PREDICTED_SETTLEMENT_PRICE:
        output = "PREDICTED_SETTLEMENT_PRICE";
        break;
      case DataFieldType::FUNDING_RATE:
        output = "FUNDING_RATE";
        break;
      case DataFieldType::NEXT_FUNDING_RATE_TIME:
        output = "NEXT_FUNDING_RATE_TIME";
        break;
      case DataFieldType::ORDER_SIDE:
        output = "ORDER_SIDE";
        break;
      case DataFieldType::ORDER_TYPE:
        output = "ORDER_TYPE";
        break;
      case DataFieldType::ORDER_FORCE:
        output = "ORDER_FORCE";
        break;
      case DataFieldType::ORDER_QTY:
        output = "ORDER_QTY";
        break;
      case DataFieldType::ORDER_PRICE:
        output = "ORDER_PRICE";
        break;
      case DataFieldType::ORDER_AVG_PRICE:
        output = "ORDER_AVG_PRICE";
        break;
      case DataFieldType::ORDER_STATUS:
        output = "ORDER_STATUS";
        break;
      case DataFieldType::ORDER_LAST_TRADE_QTY:
        output = "ORDER_LAST_TRADE_QTY";
        break;
      case DataFieldType::ORDER_CUMSUM_TRADE_QTY:
        output = "ORDER_CUMSUM_TRADE_QTY";
        break;
      default:
        CCAPI_LOGGER_FATAL(CCAPI_UNSUPPORTED_VALUE);
    }
    return output;
  }
  typedef std::map<DataFieldType, std::string> TypeForDataPoint;
  typedef std::map<DataType, std::vector<std::map<DataFieldType, std::string> > > TypeForData;
  static std::string dataToString(const TypeForData& data) {
    std::string output1 = "{";
    auto size1 = data.size();
    auto i1 = 0;
    for (const auto& elem1 : data) {
      output1 += dataTypeToString(elem1.first);
      output1 += "=";
      std::string output2 = "[ ";
      auto size2 = elem1.second.size();
      auto i2 = 0;
      for (const auto& elem2 : elem1.second) {
        std::string output3 = "{";
        auto size3 = elem2.size();
        auto i3 = 0;
        for (const auto& elem3 : elem2) {
          output3 += dataFieldTypeToString(elem3.first);
          output3 += "=";
          output3 += ccapi::toString(elem3.second);
          if (i3 < size3 - 1) {
            output3 += ", ";
          }
          ++i3;
        }
        output3 += "}";
        output2 += output3;
        if (i2 < size2 - 1) {
          output2 += ", ";
        }
        ++i2;
      }
      output2 += " ]";
      output1 += output2;
      if (i1 < size1 - 1) {
        output1 += ", ";
      }
      ++i1;
    }
    output1 += "}";
    return output1;
  }
  static std::string typeToString(Type type) {
    std::string output;
    switch (type) {
      case Type::UNKNOWN:
        output = "UNKNOWN";
        break;
      case Type::MARKET_DATA_EVENTS_MARKET_DEPTH:
        output = "MARKET_DATA_EVENTS_MARKET_DEPTH";
        break;
      case Type::MARKET_DATA_EVENTS_TRADE:
        output = "MARKET_DATA_EVENTS_TRADE";
        break;
      case Type::MARKET_DATA_EVENTS_AGG_TRADE:
        output = "MARKET_DATA_EVENTS_AGG_TRADE";
        break;
      case Type::MARKET_DATA_EVENTS_CANDLESTICK:
        output = "MARKET_DATA_EVENTS_CANDLESTICK";
        break;
      case Type::MARKET_DATA_EVENTS_MARK_PRICE:
        output = "MARKET_DATA_EVENTS_MARK_PRICE";
        break;
      default:
        CCAPI_LOGGER_FATAL(CCAPI_UNSUPPORTED_VALUE);
    }
    return output;
  }
  std::string toString() const {
    std::string output = "MarketDataMessage [type = " + typeToString(type) + ", recapType = " + recapTypeToString(recapType) + ", tp = " + ccapi::toString(tp) +
                         ", exchangeSubscriptionId = " + exchangeSubscriptionId + ", data = " + dataToString(data) + "]";
    return output;
  }
  Type type{Type::UNKNOWN};
  RecapType recapType{RecapType::UNKNOWN};
  TimePoint tp{std::chrono::seconds{0}};
  std::string exchangeSubscriptionId;
  TypeForData data;
};
} /* namespace ccapi */
#endif  // INCLUDE_CCAPI_CPP_CCAPI_MARKET_DATA_MESSAGE_H_
