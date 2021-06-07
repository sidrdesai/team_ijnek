// Copyright 2021 Kenji Brameld
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef CROUCH__CROUCH_HPP_
#define CROUCH__CROUCH_HPP_

#include <functional>
#include "motion_msgs/msg/crouch.hpp"
#include "motion_msgs/msg/ik_command.hpp"
#include "nao_interfaces/msg/joints.hpp"

class Crouch
{
public:
  Crouch(
    std::function<void(motion_msgs::msg::IKCommand)> sendIKCommand);
  void start();

private:
  std::function<void(motion_msgs::msg::IKCommand)> sendIKCommand;
  motion_msgs::msg::IKCommand command;
};

#endif  // CROUCH__CROUCH_HPP_