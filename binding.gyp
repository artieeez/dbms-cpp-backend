{
  "targets": [
    {
      "target_name": "dbms",
      "sources": [
        "src/logger/logger.cpp",
        "src/database/context.cpp",
        "src/index/trie.cpp",
        "src/index/blockStorage.cpp",
        "src/controller/linearSearchController.cpp",
        "src/controller/indexController.cpp",
        "src/controller/stateController.cpp",
        "src/node-addon/lc.cpp",
        "src/node-addon/tc.cpp",
        "src/node-addon/sc.cpp",
        "src/node-addon/dbms.cpp",
      ],
      "include_dirs": [
        "node_modules/node-addon-api",
        "src/logger",
        "src/model",
        "src/database",
        "src/index",
        "src/controller",
        "src/node-addon",
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').targets\"):node_addon_api_except",
      ]
    }
  ]
}