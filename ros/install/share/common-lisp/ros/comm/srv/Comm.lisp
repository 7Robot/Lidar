; Auto-generated. Do not edit!


(cl:in-package comm-srv)


;//! \htmlinclude Comm-request.msg.html

(cl:defclass <Comm-request> (roslisp-msg-protocol:ros-message)
  ((command
    :reader command
    :initarg :command
    :type cl:fixnum
    :initform 0))
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
  (cl:let* ((signed (cl:slot-value msg 'command)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 256) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Comm-request>) istream)
  "Deserializes a message object of type '<Comm-request>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'command) (cl:if (cl:< unsigned 128) unsigned (cl:- unsigned 256))))
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
  "e95065b3325cca9c0a0573ef228c8b1b")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Comm-request)))
  "Returns md5sum for a message object of type 'Comm-request"
  "e95065b3325cca9c0a0573ef228c8b1b")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Comm-request>)))
  "Returns full string definition for message of type '<Comm-request>"
  (cl:format cl:nil "int8 command~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Comm-request)))
  "Returns full string definition for message of type 'Comm-request"
  (cl:format cl:nil "int8 command~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Comm-request>))
  (cl:+ 0
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Comm-request>))
  "Converts a ROS message object to a list"
  (cl:list 'Comm-request
    (cl:cons ':command (command msg))
))
;//! \htmlinclude Comm-response.msg.html

(cl:defclass <Comm-response> (roslisp-msg-protocol:ros-message)
  ((error
    :reader error
    :initarg :error
    :type cl:fixnum
    :initform 0))
)

(cl:defclass Comm-response (<Comm-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <Comm-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'Comm-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name comm-srv:<Comm-response> is deprecated: use comm-srv:Comm-response instead.")))

(cl:ensure-generic-function 'error-val :lambda-list '(m))
(cl:defmethod error-val ((m <Comm-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader comm-srv:error-val is deprecated.  Use comm-srv:error instead.")
  (error m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <Comm-response>) ostream)
  "Serializes a message object of type '<Comm-response>"
  (cl:let* ((signed (cl:slot-value msg 'error)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 256) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    )
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <Comm-response>) istream)
  "Deserializes a message object of type '<Comm-response>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'error) (cl:if (cl:< unsigned 128) unsigned (cl:- unsigned 256))))
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
  "e95065b3325cca9c0a0573ef228c8b1b")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'Comm-response)))
  "Returns md5sum for a message object of type 'Comm-response"
  "e95065b3325cca9c0a0573ef228c8b1b")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<Comm-response>)))
  "Returns full string definition for message of type '<Comm-response>"
  (cl:format cl:nil "int8 error~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'Comm-response)))
  "Returns full string definition for message of type 'Comm-response"
  (cl:format cl:nil "int8 error~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <Comm-response>))
  (cl:+ 0
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <Comm-response>))
  "Converts a ROS message object to a list"
  (cl:list 'Comm-response
    (cl:cons ':error (error msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'Comm)))
  'Comm-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'Comm)))
  'Comm-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'Comm)))
  "Returns string type for a service object of type '<Comm>"
  "comm/Comm")