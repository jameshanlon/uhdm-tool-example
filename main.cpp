#include <string>
#include <memory>
#include <sys/stat.h>

#include <uhdm/uhdm.h>
#include <uhdm/vpi_listener.h>
#include <uhdm/vpi_visitor.h>
#include <uhdm/ElaboratorListener.h>


class ReportListener : public UHDM::VpiListener {
protected:
  void enterModule(const UHDM::module* object, const UHDM::BaseClass* parent,
                   vpiHandle handle, vpiHandle parentHandle) {
    auto name = vpi_get_str(vpiDefName, handle);
    std::cout << "Module name " << name << "\n";
  }
};

int main(int argc, const char *argv[]) {

  UHDM::Serializer serializer;
  std::string uhdmFile = argv[1];

  if (uhdmFile.empty()) {
    std::cerr << "No UHDM file specified\n";
    return 1;
  }

  struct stat buffer;
  if (stat(uhdmFile.c_str(), &buffer) != 0) {
    std::cerr << "File does not exist: " << uhdmFile << "\n";
    return 1;
  }

  std::vector<vpiHandle> designs = serializer.Restore(uhdmFile);

  if (designs.empty()) {
    std::cerr << "No designs\n";
    return 1;
  }

  // Elaborate the design.
  auto elabListener = std::make_unique<UHDM::ElaboratorListener>(&serializer, false);
  listen_designs(designs, elabListener.get());
  // Traverse the design.
  auto reportListener = std::make_unique<ReportListener>();
  listen_designs(designs, reportListener.get());
  return 0;
}
