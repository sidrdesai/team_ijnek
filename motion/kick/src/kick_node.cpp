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

#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "kick/kick.hpp"
#include "std_msgs/msg/empty.hpp"
#include "nao_interfaces/msg/joints.hpp"
#include "motion_msgs/msg/ik_command.hpp"
#include "motion_msgs/msg/kick.hpp"

using namespace std::placeholders;

class KickNode : public rclcpp::Node
{
public:
  KickNode()
  : Node("KickNode"),
    kick(
      std::bind(&KickNode::notifyKickDone, this),
      std::bind(&KickNode::sendIKCommand, this, _1))
  {
    sub_joint_states =
      create_subscription<nao_interfaces::msg::Joints>(
      "sensors/joints", 1,
      [this](nao_interfaces::msg::Joints::SharedPtr sensor_joints) {
        kick.notifyJoints(*sensor_joints);
      });

    sub_kick_start =
      create_subscription<motion_msgs::msg::Kick>(
      "motion/kick", 1,
      [this](motion_msgs::msg::Kick::SharedPtr kick_msg) {
        kick.start(*kick_msg);
      });

    pub_ik_command = create_publisher<motion_msgs::msg::IKCommand>("motion/ik_command", 1);
    pub_kick_done = create_publisher<std_msgs::msg::Empty>("motion/kick_done", 1);
  }

private:
  Kick kick;

  void notifyKickDone()
  {
    pub_kick_done->publish(std_msgs::msg::Empty{});
  }

  void sendIKCommand(motion_msgs::msg::IKCommand ik_command)
  {
    pub_ik_command->publish(ik_command);
  }

  rclcpp::Subscription<nao_interfaces::msg::Joints>::SharedPtr sub_joint_states;
  rclcpp::Subscription<motion_msgs::msg::Kick>::SharedPtr sub_kick_start;
  rclcpp::Publisher<motion_msgs::msg::IKCommand>::SharedPtr pub_ik_command;
  rclcpp::Publisher<std_msgs::msg::Empty>::SharedPtr pub_kick_done;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<KickNode>());
  rclcpp::shutdown();
  return 0;
}