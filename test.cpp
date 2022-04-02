#include "crow_all.h"


#include "generators/Cycle.h"
#include "generators/Complete.h"
#include "generators/Antiprism.h"
#include "generators/Prism.h"
#include "generators/Star.h"
#include "generators/Regular.h"
#include "generators/Banana.h"
#include "generators/GeneralizedPeterson.h"
#include "generators/Cmn.h"
#include "generators/Path.h"
#include "generators/Flower.h"
#include "generators/Wheel.h"
#include "generators/Crown.h"
#include "generators/Web.h"
#include "generators/CocktailParty.h"
#include "generators/Gear.h"
#include "generators/Heawood.h"

#include "reports/NumOfVertices.h"
#include "reports/NumOfEdges.h"
#include "reports/MaxDegree.h"
#include "reports/MinDegree.h"
#include "reports/MaxEigenValue.h"
#include "reports/MinEigenValue.h"
#include "reports/SumEigenValues.h"
#include "reports/Diameter.h"
#include "reports/NumOfTriangles.h"

int main()
{
    crow::SimpleApp app;
    std::vector<std::unique_ptr<GeneratorInterface>> availableGenerators;
    availableGenerators.emplace_back(std::make_unique<Antiprism>());
    availableGenerators.emplace_back(std::make_unique<Banana>());
    availableGenerators.emplace_back(std::make_unique<Cycle>());
    availableGenerators.emplace_back(std::make_unique<Complete>());
    availableGenerators.emplace_back(std::make_unique<Prism>());
    availableGenerators.emplace_back(std::make_unique<Regular>());
    availableGenerators.emplace_back(std::make_unique<GeneralizedPeterson>());
    availableGenerators.emplace_back(std::make_unique<Cmn>());
    availableGenerators.emplace_back(std::make_unique<Path>());
    availableGenerators.emplace_back(std::make_unique<Flower>());
    availableGenerators.emplace_back(std::make_unique<Star>());
    availableGenerators.emplace_back(std::make_unique<Wheel>());
    availableGenerators.emplace_back(std::make_unique<Crown>());
    availableGenerators.emplace_back(std::make_unique<Web>());
    availableGenerators.emplace_back(std::make_unique<CocktailParty>());
    availableGenerators.emplace_back(std::make_unique<Gear>());
    availableGenerators.emplace_back(std::make_unique<Heawood>());
    
    std::vector<std::unique_ptr<ReportInterface>> availableReports;
    availableReports.emplace_back(std::make_unique<NumOfVertices>());
    availableReports.emplace_back(std::make_unique<NumOfEdges>());
    availableReports.emplace_back(std::make_unique<MaxDegree>());
    availableReports.emplace_back(std::make_unique<MinDegree>());
    availableReports.emplace_back(std::make_unique<MaxEigenValue>());
    availableReports.emplace_back(std::make_unique<MinEigenValue>());
    availableReports.emplace_back(std::make_unique<SumEigenValues>());
    availableReports.emplace_back(std::make_unique<GraphDiameter>());
    availableReports.emplace_back(std::make_unique<NumberOfTriangles>());

    CROW_ROUTE(app, "/")([](){
        crow::mustache::context ctx;
    	auto main_page = crow::mustache::load("static/index.html");
    	return main_page.render();
    });

    CROW_ROUTE(app, "/graphs/")
    ([&]{
      std::vector<crow::json::wvalue> graphs;
      for (auto& gi : availableGenerators) {
        crow::json::wvalue tmp({{"name", gi->name().c_str()}});
        tmp["description"] =  gi->description().c_str();
        graphs.push_back(tmp); 
      }
      
      
      std::vector<crow::json::wvalue> reports;
      for (auto& gi : availableReports) {
        crow::json::wvalue tmp({{"name", gi->name().c_str()}});
        tmp["description"] =  gi->description().c_str();
        tmp["properties"] = "";
        reports.push_back(tmp); 
      }
      crow::json::wvalue x({{"graphs", graphs},{"reports",reports}});
//      x["reports"] = reports;
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
    
    CROW_ROUTE(app, "/add")
    .methods("POST"_method)
    ([&](const crow::request& req){
    auto x = crow::json::load(req.body);
    if (!x)
        return crow::response(crow::status::BAD_REQUEST); // same as crow::response(400)
    //int sum = x["a"].i()+x["b"].i();
    
    //std::vector<crow::json::wvalue> vs;
    std::vector<crow::json::wvalue> graph;
    for (auto& gi : availableGenerators) {
        if(gi->name().c_str() == x["name"]) {
          std::cerr << "here" << std::endl;
          std::vector<crow::json::wvalue> vs;
          auto g = gi->generate(5,3);
          for_each_v_const(g, [&](Ver v){
            crow::json::wvalue data({{"id",std::to_string(v)}});
            vs.push_back({{"data",data}});
          });
          std::vector<crow::json::wvalue> es;
          for_each_e_const(g, [&](Edge e){
            crow::json::wvalue data({{"source", std::to_string(boost::source(e,g))}, {"target",std::to_string(boost::target(e,g))}});
            es.push_back({{"data",data}});
          });
          crow::json::wvalue x({{"nodes",vs},{"edges",es}});
          crow::response response;
          response.add_header("Access-Control-Allow-Origin", "*");
          response.add_header("Access-Control-Allow-Headers", "Content-Type");
          response.add_header("Content-Type", "application/json");
          // write your data with this
          response.write(x.dump());

          return response;
        }
    }
    std::ostringstream os;
    os << "not found";
    return crow::response{os.str()};
});

    app.port(2342).multithreaded().run();
}
