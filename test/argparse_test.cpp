//
// Created by liuping on 2020/3/10.
//

#include "test_base.hpp"
#include "common/argparse.hpp"

int main(int argc, char ** argv) {
  ArgumentParser parser(argc, argv);
  parser.AddArgument({"-a", "--aarg", "arg one", true, "arg 1"});
  parser.AddArgument({"-b", "--barg", "arg two", true, "arg 2"});
  parser.AddArgument({"-c", "--carg", "arg three", true, "arg 3"});
  parser.AddArgument({"-d", "--darg", "arg four", false, "arg 4",});
  parser.AddArgument({"-e", "--earg", "arg five", false, "arg 5"});
  parser.ParseArgs();
  INFO << parser.Get("a");
  INFO << parser.Get("b");
  INFO << parser.Get("c");
  INFO << parser.Get("d");
  INFO << parser.Get("e");
  INFO << parser.Get("aarg");
  INFO << parser.Get("barg");
  INFO << parser.Get("carg");
  INFO << parser.Get("darg");
  INFO << parser.Get("earg");
}