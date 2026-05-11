import os
from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():

    # ── Find the yaml file installed in share/ ────────────────────
    #    get_package_share_directory → /install/simple_turtle_patrol/share/simple_turtle_patrol
    pkg = get_package_share_directory('simple_turtle_patrol')
    params_file = os.path.join(pkg, 'params', 'patrol_params.yaml')

    return LaunchDescription([

        # Node 1: turtlesim (from turtlesim package, not ours)
        Node(
            package='turtlesim',
            executable='turtlesim_node',
            name='turtlesim_node',
            output='screen'
        ),

        # Node 2: our status publisher
        Node(
            package='simple_turtle_patrol',
            executable='status_publisher',
            name='status_publisher',
            output='screen',
            parameters=[params_file]   # ← loads yaml into this node
        ),

        # Node 3: our patrol controller
        Node(
            package='simple_turtle_patrol',
            executable='patrol_controller',
            name='patrol_controller',
            output='screen',
            parameters=[params_file]   # ← loads yaml into this node
        ),
    ])