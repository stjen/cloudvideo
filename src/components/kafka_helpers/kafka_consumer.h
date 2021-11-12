#ifndef SRC_COMPONENTS_KAFKA_HELPERS_kafka_consumer
#define SRC_COMPONENTS_KAFKA_HELPERS_kafka_consumer

#include <atomic>
#include <kafka/KafkaConsumer.h>
#include <memory>
#include <thread>

namespace kafka_helpers {

typedef std::function<bool(const kafka::clients::consumer::ConsumerRecord &)> KafkaRecordCB;

class KafkaConsumer {
public:
  KafkaConsumer(
      KafkaRecordCB,
      kafka::Properties, std::atomic<bool> &stopConsumingFlag);
  ~KafkaConsumer() = default;
  void startConsuming(const std::string& topic);
  void consumerThread(const std::string& topic);

private:
  const uint8_t COMMIT_TIMEOUT = 1;
  std::thread m_consumerThread;
  std::atomic<bool> &m_stopConsumingFlag;
  std::unique_ptr<kafka::clients::KafkaConsumer> m_consumer;
  KafkaRecordCB m_jobCb;
};

} // namespace kafka_helpers

#endif /* SRC_COMPONENTS_KAFKA_HELPERS_kafka_consumer */
