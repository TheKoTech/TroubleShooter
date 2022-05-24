#include "cMainPingThread.h"

cMainPingThread::cMainPingThread(wxApp* handler, int timeout, std::list<wxString>* addresses) : wxThread()
{
	this->handler = handler;
	this->timeout = timeout;
    this->addressList = addresses;
}

wxThread::ExitCode cMainPingThread::Entry()
{
    // Exits loop, if the thread is asked to terminate
    while (!TestDestroy())
    {
        // The main thread loop
        wxThread::This()->Sleep(10);

        ////////////////

        // Логика этого цикла такая:
        // 1. Посылаются пакеты пинг
        // 2. Через Timeout милисекунд после начала итерации все результаты должны прийти
        // 3. (пока необяз.) Вызывается wxQueueEvent, посылающий сообщение в контроллер для отображения новых данных в графике
        wxQueueEvent(handler, new wxThreadEvent(wxEVT_THREAD, PING_THREAD_UPDATED));
        
        // 4. Результаты сохраняются логгером


        ////////////////

        // Это плохой код:
        // PingRes* res_ping = new PingRes[num_adresses];
        // MultiPing(res_ping, adresses, num_adresses, timeout);
        // Переменная результата инициализируется, и только затем заполняется в *неоднозначно* названной функции.

        // Так его можно улучшить для понимания:
        // PingRes* pingRes = MultiPing(...);

        // Или даже 
        // auto pingResults = SendPingPackages(...);
    }

    // processed by handler - cApp
    wxQueueEvent(handler, new wxThreadEvent(wxEVT_THREAD, PING_THREAD_COMPLETED));
    return (wxThread::ExitCode)0;
}

cMainPingThread::~cMainPingThread()
{
    delete addressList;
    handler = nullptr;
}

int cMainPingThread::CountAddresses()
{
    return addressList->size();
}



////// Проверка работоспособности кода //////


//for (int i = 0; i < num_adresses; ++i) {
//	Logger logger(res_ping[i].adress, res_ping[i].time);
//	logger.WriteLog();
//	logger.Check();
//}
////// конец проверки работоспособности кода //////