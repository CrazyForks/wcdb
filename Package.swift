// swift-tools-version: 5.5
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "WCDBSwift",
    platforms: [
        .macOS(.v10_13),
        .watchOS(.v4),
        .tvOS(.v12),
        .iOS(.v12)],
    products: [
        .library(name: "WCDBSwift", targets: ["WCDBSwift"]),
        .library(name: "WCDBSwiftDynamic", type: .dynamic, targets: ["WCDBSwift"]),
    ],
    dependencies: [
        .package(url: "https://github.com/Tencent/sqlcipher", .exactItem("1.4.7"))
    ],
    targets: [
        .target(
            name: "objc-core",
            path: "src/objc/core",
            sources: ["WCTFileManager.mm", "WCTFoundation.mm", "WCTFTSTokenizerUtil.mm", "WCTOperationQueue.mm"],
            publicHeadersPath:"",
            cSettings: [
//                .headerSearchPath("../../**"),
                .headerSearchPath("../../bridge"),
                .headerSearchPath("../../objc"),
                .headerSearchPath("../../bridge/objcbridge"),
                .headerSearchPath("../../bridge/cppbridge"),
                .headerSearchPath("../../bridge/winqbridge"),
                .headerSearchPath("../../bridge/base"),
                .headerSearchPath("../../bridge/winqbridge/statement"),
                .headerSearchPath("../../bridge/winqbridge/identifier"),
                .headerSearchPath("../../common/core"),
                .headerSearchPath("../../common/platform"),
                .headerSearchPath("../../common/winq"),
                .headerSearchPath("../../common/repair"),
                .headerSearchPath("../../common/base"),
                .headerSearchPath("../../common/utility"),
                .headerSearchPath("../../common/core/config"),
                .headerSearchPath("../../common/core/assemble"),
                .headerSearchPath("../../common/core/vacuum"),
                .headerSearchPath("../../common/core/cipher"),
                .headerSearchPath("../../common/core/sqlite"),
                .headerSearchPath("../../common/core/function"),
                .headerSearchPath("../../common/core/fts"),
                .headerSearchPath("../../common/core/operate"),
                .headerSearchPath("../../common/core/compression"),
                .headerSearchPath("../../common/core/integrity"),
                .headerSearchPath("../../common/core/migration"),
                .headerSearchPath("../../common/core/function/scalar"),
                .headerSearchPath("../../common/core/fts/auxfunction"),
                .headerSearchPath("../../common/core/fts/tokenizer"),
                .headerSearchPath("../../common/winq/extension"),
                .headerSearchPath("../../common/winq/statement"),
                .headerSearchPath("../../common/winq/identifier"),
                .headerSearchPath("../../common/winq/syntax"),
                .headerSearchPath("../../common/winq/syntax/stmt"),
                .headerSearchPath("../../common/winq/syntax/const"),
                .headerSearchPath("../../common/winq/syntax/identifier"),
                .headerSearchPath("../../common/repair/mechanic"),
                .headerSearchPath("../../common/repair/parse"),
                .headerSearchPath("../../common/repair/basic"),
                .headerSearchPath("../../common/repair/sqlite"),
                .headerSearchPath("../../common/repair/crawl"),
                .headerSearchPath("../../common/repair/factory"),
                .headerSearchPath("../../common/repair/utility"),
                .headerSearchPath("../../objc/transaction"),
                .headerSearchPath("../../objc/database"),
                .headerSearchPath("../../objc/monitor"),
                .headerSearchPath("../../objc/core"),
                .headerSearchPath("../../objc/handle"),
                .headerSearchPath("../../objc/bridge"),
                .headerSearchPath("../../objc/statement"),
                .headerSearchPath("../../objc/fts"),
                .headerSearchPath("../../objc/chaincall"),
                .headerSearchPath("../../objc/table"),
                .headerSearchPath("../../objc/compression"),
                .headerSearchPath("../../objc/orm"),
                .headerSearchPath("../../objc/convenient"),
                .headerSearchPath("../../objc/migration"),
                .headerSearchPath("../../objc/builtin"),
                .headerSearchPath("../../objc/orm/accessor"),
                .headerSearchPath("../../objc/orm/binding"),
                .headerSearchPath("../../objc/orm/macro"),
                .headerSearchPath("../../objc/orm/coding")
            ],
            linkerSettings: [
                .linkedFramework("UIKit", .when(platforms: [.iOS])),
            ]
        ),
        .target(
            name: "common",
            dependencies: ["sqlcipher", .target(name: "objc-core")],
            path: "src/common",
            exclude: [
                "repair/sqlite",
                "platform/WCTFileManager.cpp",
                "platform/WCTOperationQueue.cpp",
            ],
            publicHeadersPath:"",
            cSettings: [
                .define("SQLITE_WCDB", to: "1"),
                .define("SQLITE_WCDB_LOCK_HOOK", to: "1"),
                .define("SQLITE_WCDB_CHECKPOINT_HANDLER", to: "1"),
                .define("SQLITE_WCDB_SUSPEND", to: "1"),
                .define("SQLITE_WCDB_IMPROVED_CHECKPOINT", to: "1"),
                .define("SQLITE_HAS_CODEC"),
                .define("SQLITE_DEFAULT_PAGE_SIZE", to: "4096"),
                .define("SQLITE_DEFAULT_SYNCHRONOUS", to: "1"),
                .define("SQLITE_DEFAULT_WAL_SYNCHRONOUS", to: "1"),
                .define("SQLITE_DEFAULT_LOCKING_MODE", to: "0"),
                .define("SQLITE_DEFAULT_WAL_AUTOCHECKPOINT", to: "0"),
//                .headerSearchPath("**")
                .headerSearchPath("core"),
                .headerSearchPath("platform"),
                .headerSearchPath("winq"),
                .headerSearchPath("repair"),
                .headerSearchPath("base"),
                .headerSearchPath("utility"),
                .headerSearchPath("core/config"),
                .headerSearchPath("core/assemble"),
                .headerSearchPath("core/vacuum"),
                .headerSearchPath("core/cipher"),
                .headerSearchPath("core/sqlite"),
                .headerSearchPath("core/function"),
                .headerSearchPath("core/fts"),
                .headerSearchPath("core/operate"),
                .headerSearchPath("core/compression"),
                .headerSearchPath("core/integrity"),
                .headerSearchPath("core/migration"),
                .headerSearchPath("core/function/scalar"),
                .headerSearchPath("core/fts/auxfunction"),
                .headerSearchPath("core/fts/tokenizer"),
                .headerSearchPath("winq/extension"),
                .headerSearchPath("winq/statement"),
                .headerSearchPath("winq/identifier"),
                .headerSearchPath("winq/syntax"),
                .headerSearchPath("winq/syntax/stmt"),
                .headerSearchPath("winq/syntax/const"),
                .headerSearchPath("winq/syntax/identifier"),
                .headerSearchPath("repair/mechanic"),
                .headerSearchPath("repair/parse"),
                .headerSearchPath("repair/basic"),
                .headerSearchPath("repair/sqlite"),
                .headerSearchPath("repair/crawl"),
                .headerSearchPath("repair/factory"),
                .headerSearchPath("repair/utility")
            ],
            linkerSettings: [
                .linkedLibrary("z"),
            ]
        ),
        .target(
            name: "bridge",
            dependencies: [.target(name: "common")],
            path: "src/bridge",
            exclude: [
                "tests",
            ],
            cSettings: [
//                .headerSearchPath("../**")
                .headerSearchPath("../bridge"),
                .headerSearchPath("../objc"),
                .headerSearchPath("../bridge/objcbridge"),
                .headerSearchPath("../bridge/cppbridge"),
                .headerSearchPath("../bridge/winqbridge"),
                .headerSearchPath("../bridge/base"),
                .headerSearchPath("../bridge/winqbridge/statement"),
                .headerSearchPath("../bridge/winqbridge/identifier"),
                .headerSearchPath("../common/core"),
                .headerSearchPath("../common/platform"),
                .headerSearchPath("../common/winq"),
                .headerSearchPath("../common/repair"),
                .headerSearchPath("../common/base"),
                .headerSearchPath("../common/utility"),
                .headerSearchPath("../common/core/config"),
                .headerSearchPath("../common/core/assemble"),
                .headerSearchPath("../common/core/vacuum"),
                .headerSearchPath("../common/core/cipher"),
                .headerSearchPath("../common/core/sqlite"),
                .headerSearchPath("../common/core/function"),
                .headerSearchPath("../common/core/fts"),
                .headerSearchPath("../common/core/operate"),
                .headerSearchPath("../common/core/compression"),
                .headerSearchPath("../common/core/integrity"),
                .headerSearchPath("../common/core/migration"),
                .headerSearchPath("../common/core/function/scalar"),
                .headerSearchPath("../common/core/fts/auxfunction"),
                .headerSearchPath("../common/core/fts/tokenizer"),
                .headerSearchPath("../common/winq/extension"),
                .headerSearchPath("../common/winq/statement"),
                .headerSearchPath("../common/winq/identifier"),
                .headerSearchPath("../common/winq/syntax"),
                .headerSearchPath("../common/winq/syntax/stmt"),
                .headerSearchPath("../common/winq/syntax/const"),
                .headerSearchPath("../common/winq/syntax/identifier"),
                .headerSearchPath("../common/repair/mechanic"),
                .headerSearchPath("../common/repair/parse"),
                .headerSearchPath("../common/repair/basic"),
                .headerSearchPath("../common/repair/sqlite"),
                .headerSearchPath("../common/repair/crawl"),
                .headerSearchPath("../common/repair/factory"),
                .headerSearchPath("../common/repair/utility"),
                .headerSearchPath("../objc/transaction"),
                .headerSearchPath("../objc/database"),
                .headerSearchPath("../objc/monitor"),
                .headerSearchPath("../objc/core"),
                .headerSearchPath("../objc/handle"),
                .headerSearchPath("../objc/bridge"),
                .headerSearchPath("../objc/statement"),
                .headerSearchPath("../objc/fts"),
                .headerSearchPath("../objc/chaincall"),
                .headerSearchPath("../objc/table"),
                .headerSearchPath("../objc/compression"),
                .headerSearchPath("../objc/orm"),
                .headerSearchPath("../objc/convenient"),
                .headerSearchPath("../objc/migration"),
                .headerSearchPath("../objc/builtin"),
                .headerSearchPath("../objc/orm/accessor"),
                .headerSearchPath("../objc/orm/binding"),
                .headerSearchPath("../objc/orm/macro"),
                .headerSearchPath("../objc/orm/coding")
            ]
        ),
        .target(
            name: "WCDBSwift",
            dependencies: [.target(name: "bridge")],
            path: "src/swift",
            exclude: [
                "tests"
            ],
            publicHeadersPath: ".",
            cSettings: [
//                .headerSearchPath("../bridge/**")
                .headerSearchPath("../bridge"),
                .headerSearchPath("../bridge/objcbridge"),
                .headerSearchPath("../bridge/cppbridge"),
                .headerSearchPath("../bridge/winqbridge"),
                .headerSearchPath("../bridge/base"),
                .headerSearchPath("../bridge/winqbridge/statement"),
                .headerSearchPath("../bridge/winqbridge/identifier")
            ]
        )
    ],
    cLanguageStandard: .c11,
    cxxLanguageStandard: .cxx14
)
