#ifndef SRC_COMPONENTS_KAFKA_HELPERS_kafka_producer
#define SRC_COMPONENTS_KAFKA_HELPERS_kafka_producer

#include <atomic>
#include <kafka/KafkaProducer.h>
#include <thread>
#include <util/atomic_queue.h>

namespace kafka_helpers {

class KafkaProducer {
public:
  KafkaProducer(components::util::AtomicQueue<std::string> &,
                   kafka::Properties, std::atomic<bool> &stopProducingFlag);
  ~KafkaProducer() = default;
  void startProducing(const std::string &topic);
  void producerThread(const std::string &topic);


private:
  components::util::AtomicQueue<std::string> &m_producerQueue;
  std::thread m_producerThread;
  std::atomic<bool> &m_stopProducingFlag;
  std::unique_ptr<kafka::clients::KafkaProducer> m_producer;
  kafka::Properties m_props;
};

} // namespace kafka_helpers

#endif /* SRC_COMPONENTS_KAFKA_HELPERS_kafka_producer */
