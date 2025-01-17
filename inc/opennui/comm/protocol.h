#ifndef _OPENNUI_SDK4_COMM_PROTOCOL_H_
#define _OPENNUI_SDK4_COMM_PROTOCOL_H_

#include <vee/enumeration.h>
#include <array>

namespace opennui {

namespace protocol {

namespace comm {

Enumeration(comm_port, 12835,
            native_comm_port,
            web_comm_port);

Enumeration(opcode_t, 500,
            null,
            handshake_hello,
            handshake_ack,
            end_of_record);

Enumeration(client_type, 250,
            null,
            native,
            web);

const size_t pipe_io_buffer_size = 1024;
const unsigned long pipe_server_timeout = 5000;
const unsigned long pipe_client_timeout = 5000;

#pragma pack(push, 1)
struct message_header
{
    opcode_t opcode = opcode_t::null;
    uint32_t block_size = 0;
    uint32_t message_id = 0;
};
#pragma pack(pop)

struct message
{
    static const int data_section_max = 512;
    message_header header;
    ::std::array<unsigned char, data_section_max> data_section;
    inline void clear()
    {
        memset(&header, 0, sizeof(header));
        data_section.fill(0);
    }
    inline uint32_t to_binary(unsigned char* dst)
    {
        memmove(dst, &header, sizeof(message_header));
        memmove(dst + sizeof(message_header), &data_section, header.block_size);
        return sizeof(message_header) + header.block_size;
    }
};

void validate_header(message_header& header) throw(...);

} // !namespace comm

} // !namespace protocol

} // !namepsace opennui

#endif // !_OPENNUI_COMM_PROTOCOL_H_
