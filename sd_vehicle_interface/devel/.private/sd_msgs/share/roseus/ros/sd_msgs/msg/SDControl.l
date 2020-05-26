;; Auto-generated. Do not edit!


(when (boundp 'sd_msgs::SDControl)
  (if (not (find-package "SD_MSGS"))
    (make-package "SD_MSGS"))
  (shadow 'SDControl (find-package "SD_MSGS")))
(unless (find-package "SD_MSGS::SDCONTROL")
  (make-package "SD_MSGS::SDCONTROL"))

(in-package "ROS")
;;//! \htmlinclude SDControl.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass sd_msgs::SDControl
  :super ros::object
  :slots (_header _torque _steer ))

(defmethod sd_msgs::SDControl
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:torque __torque) 0.0)
    ((:steer __steer) 0.0)
    )
   (send-super :init)
   (setq _header __header)
   (setq _torque (float __torque))
   (setq _steer (float __steer))
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:torque
   (&optional __torque)
   (if __torque (setq _torque __torque)) _torque)
  (:steer
   (&optional __steer)
   (if __steer (setq _steer __steer)) _steer)
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; float64 _torque
    8
    ;; float64 _steer
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; float64 _torque
       (sys::poke _torque (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _steer
       (sys::poke _steer (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; float64 _torque
     (setq _torque (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _steer
     (setq _steer (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(setf (get sd_msgs::SDControl :md5sum-) "8061ac3467db06e9c1e267e9bb9a33f8")
(setf (get sd_msgs::SDControl :datatype-) "sd_msgs/SDControl")
(setf (get sd_msgs::SDControl :definition-)
      "Header header

# Range -100 to 100, -100 is max brake, +100 is max throttle
float64 torque

# Range -100 to +100, +100 is maximum left turn
float64 steer

================================================================================
MSG: std_msgs/Header
# Standard metadata for higher-level stamped data types.
# This is generally used to communicate timestamped data 
# in a particular coordinate frame.
# 
# sequence ID: consecutively increasing ID 
uint32 seq
#Two-integer timestamp that is expressed as:
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
# time-handling sugar is provided by the client library
time stamp
#Frame this data is associated with
# 0: no frame
# 1: global frame
string frame_id

")



(provide :sd_msgs/SDControl "8061ac3467db06e9c1e267e9bb9a33f8")


