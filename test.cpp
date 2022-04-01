#include "crow_all.h"

int main()
{
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        crow::mustache::context ctx;
    	auto main_page = crow::mustache::load("static/index.html");
    	return main_page.render();
    });

    CROW_ROUTE(app, "/graphs/")
    ([]{
      std::vector<crow::json::wvalue> graphs;
      crow::json::wvalue tmp({{"name", "test"}});
      graphs.push_back(tmp);
      crow::json::wvalue x({{"graphs", graphs}});
//      x["graphs"] = graphs;
      crow::response response;
      response.add_header("Access-Control-Allow-Origin", "*");
      response.add_header("Access-Control-Allow-Headers", "Content-Type");
      response.add_header("Content-Type", "application/json");
      // write your data with this
      response.write(x.dump());

      return response;
  //    return x;
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

    app.port(2342).multithreaded().run();
}
