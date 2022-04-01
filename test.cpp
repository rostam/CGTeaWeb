#include "crow_all.h"

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        crow::mustache::context ctx;
    	auto main_page = crow::mustache::load("../static/index.html");
    	return main_page.render();
    });
    
    CROW_ROUTE(app, "/json")
    ([]{
      crow::json::wvalue x({{"message", "Hello, World!"}});
      x["message2"] = "Hello, World.. Again!";
      return x;
    });

    CROW_ROUTE(app,"/hello/<int>")
    ([](int count){
      return crow::response(std::to_string(count));
    });

    app.port(18080).multithreaded().run();
}
