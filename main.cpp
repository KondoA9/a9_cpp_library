#include "filesystem/filesystem.h"
#include "console/console.h"
#include "csv/csv.h"

int main(){
    auto cont = a9::filesystem::get_directory_contents(".");

    a9::csv csv("aa.csv");
    if (csv) {

        auto a = csv[0];
        auto b = csv[0];

        csv.close();
    }

    /*const auto contents = a9::celeritas::fs::get_directory_contents("folder", true);

    a9::celeritas::util::remove_if(contents, [](const std::string& s) {return a9::celeritas::fs::is_directory(s); });

    a9::console::print(a9::console::type::information, std::to_string(contents.size()));*/
}