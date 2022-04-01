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
    ([&availableGenerators]{
      std::vector<crow::json::wvalue> graphs;
      for (auto& gi : availableGenerators) {
        crow::json::wvalue tmp({{"name", gi->name().c_str()}});
        graphs.push_back(tmp); 
//        menuGenerate->Append(i, wxString(gi->name().c_str(), wxConvUTF8),
//                             wxString(gi->description().c_str(), wxConvUTF8));
      }
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
    
    
    CROW_ROUTE(app, "/reports/")
    ([&availableGenerators]{
      std::vector<crow::json::wvalue> graphs;
      for (auto& gi : availableGenerators) {
        crow::json::wvalue tmp({{"name", gi->name().c_str()}});
        graphs.push_back(tmp); 
//        menuGenerate->Append(i, wxString(gi->name().c_str(), wxConvUTF8),
//                             wxString(gi->description().c_str(), wxConvUTF8));
      }
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
