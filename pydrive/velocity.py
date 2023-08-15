import sys

class Detection:
    def __init__(self, position, velocity):
        self._position =  position
        self._velocity = velocity

    @property
    def position(self):
        return self._position

    @property
    def velocity(self):
        return self._velocity


radar_detection = Detection((120.274666, -0.299558), (0.401704, -0.889495))

print(radar_detection.position)
print(radar_detection.velocity)

class Velocity:
    def __init__(self, vx, vy):
        self._vx = vx
        self._vy = vy

    def __sub__(self, other):
        return Velocity(self._vx - other._vx, self._vy - other._vy)

    def __add__(self, other):
        return Velocity(self._vx + other._vx, self._vy + other._vy)

    def __repr__(self):
        return "Velocity(x={}, y={})".format(self._vx, self._vy)

v0 = Velocity(0, 0)
v1 = Velocity(1, 1)
v01 = v0 - v1
v10 = v1 - v0
print(v01)
print(v10)

det_v = Velocity(-21.747866, -0.806919)
transport_v = Velocity(-0.000206, -0.082576)
rotate_v = Velocity(22.149775, -0.000000)

ego_velocity= Velocity(22.097936, -1.514518) #大地坐标系下的


# 比较 +/- transport_v 影响并不大，因此还是yaw自身带来的问题
res = det_v + rotate_v + transport_v
res0 = det_v + rotate_v - transport_v
print(res)
print(res0)

# det_v 是个静态的物体，与 ego_velocity 只能是相反的方向
# 所以他们两个有一个的速度是错误的:
# suppose 1, radar is wrong, det_v. Why? What is it caused by?
# suppose 2, ego_velocity is wrong. <-- IMU

res_diff = det_v + ego_velocity
print(res_diff)


sys.exit()
#
#def compuateEgoYaw(ego_pose):
#    if std::hypot(ego_pose.vx, ego_pose.vy) < 5.0:
#        return ego_pose.yaw
#    else:
#        ego_yaw = atan2(ego_pose.vy, ego_pose.vx)
#        return ego_yaw
#
#
#def computeRadarDetectionVelocity(detection_velocity, detection_position, ego_pose, rotate_to_imu):
#    #     // angular velocity of IMU frame, relative to ground.
#    #Eigen::Vector3d angular_velocity(0.0, 0.0, yaw_rate);
#
#    #// transport velocity caused by angular velocity of IMU frame,
#    #// here we simply the condition as: radar detection is on xy-plane,
#    #// and the rotation of imu frame is only w.r.t z-axis
#    #Eigen::Vector3d transport_velocity = angular_velocity.cross(position_in_imu);
#
#    rotate_to_imu =
#    ego_velocity(ego_pose.vx, ego_pose.vy, 0.0);
#
#    return detection_velocity + rotate_to_imu * ego_pose_velocity + transport_velocity
