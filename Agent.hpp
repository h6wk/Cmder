
#include <memory>

class Agent
{
public:
  using SharedPtr = std::shared_ptr<Agent>;

  virtual ~Agent();

  /// @brief Create a new Agent object.
  /// @return Shared pointer on the newly created object.
  static SharedPtr create();

protected:
  Agent();



};
