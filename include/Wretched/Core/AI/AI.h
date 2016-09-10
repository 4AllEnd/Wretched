#pragma once
#include <vector>
namespace AI {
  //"inputs"
  struct State {
    std::vector<double>values;
  };
  //"outputs"
  struct Action {
    double reward;
    double qvalue;
  };
  //"results"
  struct QValue {
    
  };

  struct Node {
    State state;
  };

  struct Agent {
    std::vector<std::pair<std::vector<State>,float>>state_action_reward;
  };
  struct ActorCritic: public Agent{};
}