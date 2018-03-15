; Auto-generated. Do not edit!


(cl:in-package comm-srv)


;//! \htmlinclude Comm-request.msg.html

(cl:defclass <Comm-request> (roslisp-msg-protocol:ros-message)
  ((command
    :reader command
    :initarg :command
    :type cl:string
    :initform ""))
)

(cl:defclass Comm-request (<Comm-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Comm-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Comm-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm-srv:<Comm-request> is deprecated: use comm-srv:Comm-request instead.")))

(cl:ensure-generic-function 'command-val :lambda-list '(m))
(cl:defmethod command-val ((m <Comm-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm-srv:command-val is deprecated.  Use comm-srv:command instead.")
  (command m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Comm-request>) ostream)
  "Serializes a message object of type '<Comm-request>"
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'command))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'command))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Comm-request>) istream)
  "Deserializes a message object of type '<Comm-request>"
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'command) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'command) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Comm-request>)))
  "Returns string type for a service object of type '<Comm-request>"
  "comm/CommRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Comm-request)))
  "Returns string type for a service object of type 'Comm-request"
  "comm/CommRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Comm-request>)))
  "Returns md5sum for a message object of type '<Comm-request>"
  "0fa1ccfd0a5a431161710f45bbf0130a")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Comm-request)))
  "Returns md5sum for a message object of type 'Comm-request"
  "0fa1ccfd0a5a431161710f45bbf0130a")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Comm-request>)))
  "Returns full string definition for message of type '<Comm-request>"
  (cl:format cl:nil "string command~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Comm-request)))
  "Returns full string definition for message of type 'Comm-request"
  (cl:format cl:nil "string command~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Comm-request>))
  (cl:+ 0
     4 (cl:length (cl:slot-value msg 'command))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Comm-request>))
  "Converts a ROS message object to a list"
  (cl:list 'Comm-request
    (cl:cons ':command (command msg))
))
;//! \htmlinclude Comm-response.msg.html

(cl:defclass <Comm-response> (roslisp-msg-protocol:ros-message)
  ((answer
    :reader answer
    :initarg :answer
    :type cl:string
    :initform ""))
)

(cl:defclass Comm-response (<Comm-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Comm-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Comm-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm-srv:<Comm-response> is deprecated: use comm-srv:Comm-response instead.")))

(cl:ensure-generic-function 'answer-val :lambda-list '(m))
(cl:defmethod answer-val ((m <Comm-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm-srv:answer-val is deprecated.  Use comm-srv:answer instead.")
  (answer m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Comm-response>) ostream)
  "Serializes a message object of type '<Comm-response>"
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'answer))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'answer))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Comm-response>) istream)
  "Deserializes a message object of type '<Comm-response>"
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'answer) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'answer) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<Comm-response>)))
  "Returns string type for a service object of type '<Comm-response>"
  "comm/CommResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Comm-response)))
  "Returns string type for a service object of type 'Comm-response"
  "comm/CommResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<Comm-response>)))
  "Returns md5sum for a message object of type '<Comm-response>"
  "0fa1ccfd0a5a431161710f45bbf0130a")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Comm-response)))
  "Returns md5sum for a message object of type 'Comm-response"
  "0fa1ccfd0a5a431161710f45bbf0130a")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Comm-response>)))
  "Returns full string definition for message of type '<Comm-response>"
  (cl:format cl:nil "string answer~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Comm-response)))
  "Returns full string definition for message of type 'Comm-response"
  (cl:format cl:nil "string answer~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Comm-response>))
  (cl:+ 0
     4 (cl:length (cl:slot-value msg 'answer))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Comm-response>))
  "Converts a ROS message object to a list"
  (cl:list 'Comm-response
    (cl:cons ':answer (answer msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'Comm)))
  'Comm-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'Comm)))
  'Comm-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Comm)))
  "Returns string type for a service object of type '<Comm>"
  "comm/Comm")