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
    ((:command __command) "")
    )
   (send-super :init)
   (setq _command (string __command))
   self)
  (:command
   (&optional __command)
   (if __command (setq _command __command)) _command)
  (:serialization-length
   ()
   (+
    ;; string _command
    4 (length _command)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; string _command
       (write-long (length _command) s) (princ _command s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; string _command
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _command (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;;
   self)
  )

(defclass comm::CommResponse
  :super ros::object
  :slots (_answer ))

(defmethod comm::CommResponse
  (:init
   (&key
    ((:answer __answer) "")
    )
   (send-super :init)
   (setq _answer (string __answer))
   self)
  (:answer
   (&optional __answer)
   (if __answer (setq _answer __answer)) _answer)
  (:serialization-length
   ()
   (+
    ;; string _answer
    4 (length _answer)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; string _answer
       (write-long (length _answer) s) (princ _answer s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; string _answer
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _answer (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;;
   self)
  )

(defclass comm::Comm
  :super ros::object
  :slots ())

(setf (get comm::Comm :md5sum-) "0fa1ccfd0a5a431161710f45bbf0130a")
(setf (get comm::Comm :datatype-) "comm/Comm")
(setf (get comm::Comm :request) comm::CommRequest)
(setf (get comm::Comm :response) comm::CommResponse)

(defmethod comm::CommRequest
  (:response () (instance comm::CommResponse :init)))

(setf (get comm::CommRequest :md5sum-) "0fa1ccfd0a5a431161710f45bbf0130a")
(setf (get comm::CommRequest :datatype-) "comm/CommRequest")
(setf (get comm::CommRequest :definition-)
      "string command
---
string answer

")

(setf (get comm::CommResponse :md5sum-) "0fa1ccfd0a5a431161710f45bbf0130a")
(setf (get comm::CommResponse :datatype-) "comm/CommResponse")
(setf (get comm::CommResponse :definition-)
      "string command
---
string answer

")



(provide :comm/Comm "0fa1ccfd0a5a431161710f45bbf0130a")


