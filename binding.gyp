{
  "targets": [
    {
      "target_name": "dbms",
      "sources": [
        "src/database/context.cpp",
        "src/controller/linearSearchController.cpp",
        "src/napi/dbms.cpp",
      ],
      "include_dirs": [
        "node_modules/node-addon-api",
        "src/model",
        "src/database",
        "src/index",
        "src/controller",
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').targets\"):node_addon_api_except",
      ]
    }
  ]
}