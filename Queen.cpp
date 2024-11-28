#include "Queen.hpp"

int main(int argc, char* argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QCoreApplication::setOrganizationName("Cadabra Systems");
	QCoreApplication::setOrganizationDomain("cadabra.systems");
	QCoreApplication::setApplicationVersion(QUEEN_FRAMEWORK_VERSION);
	return SkeletonMain(argc, argv);
}
