// Generated by gencpp from file comm/CommResponse.msg
// DO NOT EDIT!


#ifndef COMM_MESSAGE_COMMRESPONSE_H
#define COMM_MESSAGE_COMMRESPONSE_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace comm
{
template <class ContainerAllocator>
struct CommResponse_
{
  typedef CommResponse_<ContainerAllocator> Type;

  CommResponse_()
    : answer()  {
    }
  CommResponse_(const ContainerAllocator& _alloc)
    : answer(_alloc)  {
  (void)_alloc;
    }



   typedef std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other >  _answer_type;
  _answer_type answer;




  typedef boost::shared_ptr< ::comm::CommResponse_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::comm::CommResponse_<ContainerAllocator> const> ConstPtr;

}; // struct CommResponse_

typedef ::comm::CommResponse_<std::allocator<void> > CommResponse;

typedef boost::shared_ptr< ::comm::CommResponse > CommResponsePtr;
typedef boost::shared_ptr< ::comm::CommResponse const> CommResponseConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::comm::CommResponse_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::comm::CommResponse_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace comm

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': False}
// {'std_msgs': ['/opt/ros/kinetic/share/std_msgs/cmake/../msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::comm::CommResponse_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::comm::CommResponse_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::comm::CommResponse_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::comm::CommResponse_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::comm::CommResponse_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::comm::CommResponse_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::comm::CommResponse_<ContainerAllocator> >
{
  static const char* value()
  {
    return "d7e708f879c94bb931716d8f4f130f30";
  }

  static const char* value(const ::comm::CommResponse_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0xd7e708f879c94bb9ULL;
  static const uint64_t static_value2 = 0x31716d8f4f130f30ULL;
};

template<class ContainerAllocator>
struct DataType< ::comm::CommResponse_<ContainerAllocator> >
{
  static const char* value()
  {
    return "comm/CommResponse";
  }

  static const char* value(const ::comm::CommResponse_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::comm::CommResponse_<ContainerAllocator> >
{
  static const char* value()
  {
    return "string answer\n\
\n\
";
  }

  static const char* value(const ::comm::CommResponse_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::comm::CommResponse_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.answer);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct CommResponse_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::comm::CommResponse_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::comm::CommResponse_<ContainerAllocator>& v)
  {
    s << indent << "answer: ";
    Printer<std::basic_string<char, std::char_traits<char>, typename ContainerAllocator::template rebind<char>::other > >::stream(s, indent + "  ", v.answer);
  }
};

} // namespace message_operations
} // namespace ros

#endif // COMM_MESSAGE_COMMRESPONSE_H
