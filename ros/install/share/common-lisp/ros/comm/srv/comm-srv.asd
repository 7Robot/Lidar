
(cl:in-package :asdf)

(defsystem "comm-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "Comm" :depends-on ("_package_Comm"))
    (:file "_package_Comm" :depends-on ("_package"))
  ))