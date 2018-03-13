;; Auto-generated. Do not edit!


(when (boundp 'comm::Comm)
  (if (not (find-package "COMM"))
    (make-package "COMM"))
  (shadow 'Comm (find-package "COMM")))
(unless (find-package "COMM::COMM")
  (make-package "COMM::COMM"))
(unless (find-package "COMM::COMMREQUEST")
  (make-package "COMM::COMMREQUEST"))
(unless (find-package "COMM::COMMRESPONSE")
  (make-package "COMM::COMMRESPONSE"))

(in-package "ROS")





(defclass comm::CommRequest
  :super ros::object
  :slots (_command ))

(defmethod comm::CommRequest
  (:init
   (&key
    ((:command __command) 0)
    )
   (send-super :init)
   (setq _command (round __command))
   self)
  (:command
   (&optional __command)
   (if __command (setq _command __command)) _command)
  (:serialization-length
   ()
   (+
    ;; int8 _command
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int8 _command
       (write-byte _command s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int8 _command
     (setq _command (sys::peek buf ptr- :char)) (incf ptr- 1)
     (if (> _command 127) (setq _command (- _command 256)))
   ;;
   self)
  )

(defclass comm::CommResponse
  :super ros::object
  :slots (_error ))

(defmethod comm::CommResponse
  (:init
   (&key
    ((:error __error) 0)
    )
   (send-super :init)
   (setq _error (round __error))
   self)
  (:error
   (&optional __error)
   (if __error (setq _error __error)) _error)
  (:serialization-length
   ()
   (+
    ;; int8 _error
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int8 _error
       (write-byte _error s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int8 _error
     (setq _error (sys::peek buf ptr- :char)) (incf ptr- 1)
     (if (> _error 127) (setq _error (- _error 256)))
   ;;
   self)
  )

(defclass comm::Comm
  :super ros::object
  :slots ())

(setf (get comm::Comm :md5sum-) "e95065b3325cca9c0a0573ef228c8b1b")
(setf (get comm::Comm :datatype-) "comm/Comm")
(setf (get comm::Comm :request) comm::CommRequest)
(setf (get comm::Comm :response) comm::CommResponse)

(defmethod comm::CommRequest
  (:response () (instance comm::CommResponse :init)))

(setf (get comm::CommRequest :md5sum-) "e95065b3325cca9c0a0573ef228c8b1b")
(setf (get comm::CommRequest :datatype-) "comm/CommRequest")
(setf (get comm::CommRequest :definition-)
      "int8 command
---
int8 error

")

(setf (get comm::CommResponse :md5sum-) "e95065b3325cca9c0a0573ef228c8b1b")
(setf (get comm::CommResponse :datatype-) "comm/CommResponse")
(setf (get comm::CommResponse :definition-)
      "int8 command
---
int8 error

")



(provide :comm/Comm "e95065b3325cca9c0a0573ef228c8b1b")


