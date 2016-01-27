#include <opennui/opennui.h>
#include <opennui/clnt/comm/session.h>
#include <opennui/clnt/logger.h>
#include <opennui/clnt/api.h>
#include <list>
#include <conio.h>
#include <Windows.h>
#pragma warning(disable:4127)
#pragma warning(disable:4996)

int main()
{
    namespace api = ::opennui::clnt::api;
    namespace comm = ::opennui::clnt::comm;
    using logger = ::opennui::clnt::logger;
    comm::native_session::shared_ptr session_handle;
    try
    {
        session_handle = api::connect_to_framework();
    }
    catch (::vee::exception& e)
    {
        logger::system_error_log(e.to_string());
    }
    auto hcsbuf = api::temp_get_color_buffer();
    logger::system_msg(0x0E, (char*)hcsbuf->get_address());
    unsigned char* buffer = new unsigned char[1920 * 1080 * 4]; // RGBA Raw Image
    while (true)
    {
        printf("press enter key to grab a new color image...\n");
        while (true)
        {
            int ch = getch();
            if (ch == 13)
                break;
            else if (ch == 27)
            {
                delete[]buffer;
                return 0;
            }
        }
        api::temp_acquire_kinect2_color_frame(hcsbuf, buffer);
        for (int i = 1; i <= 40; ++i)
        {
            printf("0x%02X", (char)buffer[i]);
            if (i % 10 == 0)
                printf("\n");
            else
                printf(" ");
        }
        printf("\n");
    }
}

int xmain()
{
    ::std::list<::opennui::clnt::comm::native_session::shared_ptr> sessions;
    try
    {
        while (true)
        {
            {
                auto cs = ::opennui::clnt::comm::connect_to_local_framework();
                sessions.push_back(cs);
            }
            printf("press enter key to create new client...\n");
            while (true)
            {
                int ch = getch();
                if (ch == 13)
                    break;
                else if (ch == 27)
                {
                    return 0;
                }
                else if (ch == 'c')
                {
                    sessions.clear();
                    continue;
                }
            }
        }
    }
    catch (::vee::exception& e)
    {
        printf("err> %s\n", e.to_string());
    }
    return 0;
}