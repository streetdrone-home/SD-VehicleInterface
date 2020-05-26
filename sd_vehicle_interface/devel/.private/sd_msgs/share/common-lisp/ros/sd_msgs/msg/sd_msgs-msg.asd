
(cl:in-package :asdf)

(defsystem "sd_msgs-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "SDControl" :depends-on ("_package_SDControl"))
    (:file "_package_SDControl" :depends-on ("_package"))
  ))