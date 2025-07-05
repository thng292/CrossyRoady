const std = @import("std");
const rl = @import("raylib");
const builtin = @import("builtin");

const emccOutputDir = "zig-out/emcc";

pub fn build(b: *std.Build) !void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const raylib = getRaylib(b, target, optimize, rl.Options.getOptions(b));
    const console_engine_lib = try getConsoleGameLib(b, target, optimize, raylib);
    const run_step = b.step("run", "Run the app");
    const build_console_engine_only = b.option(bool, "console_lib_only", "Only build the ConsoleEngine") orelse false;

    if (build_console_engine_only) {
        b.installArtifact(console_engine_lib);
        return;
    }

    const build_step = try if (target.result.os.tag == .emscripten)
        buildEnscripten(b, target, optimize, console_engine_lib, raylib)
    else
        buildNative(b, target, optimize, console_engine_lib);

    run_step.dependOn(build_step);
}

fn buildNative(
    b: *std.Build,
    target: std.Build.ResolvedTarget,
    optimize: std.builtin.OptimizeMode,
    console_engine_lib: *std.Build.Step.Compile,
) !*std.Build.Step {
    var exe_mod = b.createModule(.{
        .target = target,
        .optimize = optimize,
        .link_libcpp = true,
    });
    try addAllCppSources(b, exe_mod, "CrossyRoady/");
    exe_mod.addIncludePath(b.path("CrossyRoady/"));
    exe_mod.linkLibrary(console_engine_lib);

    const exe = b.addExecutable(.{
        .name = "CrossyRoady",
        .root_module = exe_mod,
    });
    b.installArtifact(exe);

    const install = b.getInstallStep();
    const install_data = b.addInstallDirectory(.{
        .source_dir = b.path("CrossyRoady/resource/"),
        .install_dir = .{ .bin = {} },
        .install_subdir = "resource/",
    });
    install.dependOn(&install_data.step);

    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }
    return &run_cmd.step;
}

fn buildEnscripten(
    b: *std.Build,
    target: std.Build.ResolvedTarget,
    optimize: std.builtin.OptimizeMode,
    console_engine_lib: *std.Build.Step.Compile,
    raylib: *std.Build.Step.Compile,
) !*std.Build.Step {
    if (b.lazyDependency("emsdk", .{})) |emsdk_dep| {
        var exe_mod = b.addStaticLibrary(.{
            .name = "CrossyRoady",
            .target = target,
            .optimize = optimize,
        });
        try addAllCppSources(b, exe_mod.root_module, "CrossyRoady/");
        exe_mod.addIncludePath(b.path("CrossyRoady/"));
        exe_mod.linkLibrary(console_engine_lib);

        if (try emSdkSetupStep(b, emsdk_dep)) |emSdkStep| {
            exe_mod.step.dependOn(&emSdkStep.step);
        }
        exe_mod.addSystemIncludePath(emsdk_dep.path("upstream/emscripten/cache/sysroot/include"));
        exe_mod.linkLibCpp();

        // Create the output directory because emcc can't do it.
        const emccOutputDirExample = b.pathJoin(&.{ emccOutputDir, "CrossyRoady", std.fs.path.sep_str });
        const mkdir_command = switch (builtin.os.tag) {
            .windows => b.addSystemCommand(&.{ "cmd.exe", "/c", "if", "not", "exist", emccOutputDirExample, "mkdir", emccOutputDirExample }),
            else => b.addSystemCommand(&.{ "mkdir", "-p", emccOutputDirExample }),
        };

        const emcc_exe = switch (builtin.os.tag) {
            .windows => "emcc.bat",
            else => "emcc",
        };

        const module_resources = "CrossyRoady/resource@resource";

        const emcc_exe_path = b.pathJoin(&.{ emsdk_dep.path("upstream/emscripten").getPath(b), emcc_exe });
        const emcc_command = b.addSystemCommand(&[_][]const u8{emcc_exe_path});
        emcc_command.step.dependOn(&mkdir_command.step);
        const emccOutputDirExampleWithFile = b.pathJoin(&.{ emccOutputDir, "CrossyRoady", std.fs.path.sep_str, "index.html" });
        emcc_command.addArgs(&[_][]const u8{
            "-o",
            emccOutputDirExampleWithFile,
            "-std=c++20",
            "-sFULL-ES3=1",
            "-sUSE_GLFW=3",
            "-sSTACK_OVERFLOW_CHECK=1",
            "-sEXPORTED_RUNTIME_METHODS=['requestFullscreen']",
            "-sASYNCIFY",
            "-O3",
            "-sASSERTIONS",
            "-sALLOW_MEMORY_GROWTH=1",
            "--emrun",
            "-error-limit=0",
            "--preload-file",
            module_resources,
            "--shell-file",
            b.path("shell.html").getPath(b),
        });

        const link_items: []const *std.Build.Step.Compile = &.{
            console_engine_lib,
            exe_mod,
            raylib,
        };
        for (link_items) |item| {
            emcc_command.addFileArg(item.getEmittedBin());
            emcc_command.step.dependOn(&item.step);
        }

        b.getInstallStep().dependOn(&emcc_command.step);

        const em_run_step = try emscriptenRunStep(b, emsdk_dep, emccOutputDirExampleWithFile);
        em_run_step.step.dependOn(&emcc_command.step);
        em_run_step.addArg("--no_browser");

        return &em_run_step.step;
    }
    unreachable;
}

fn getConsoleGameLib(
    b: *std.Build,
    target: std.Build.ResolvedTarget,
    optimize: std.builtin.OptimizeMode,
    raylib: *std.Build.Step.Compile,
) !*std.Build.Step.Compile {
    var console_engine_lib = b.addStaticLibrary(.{
        .name = "ConsoleEngine",
        .target = target,
        .optimize = optimize,
    });

    if (target.result.os.tag == .emscripten) {
        if (b.lazyDependency("emsdk", .{})) |dep| {
            if (try emSdkSetupStep(b, dep)) |emSdkStep| {
                console_engine_lib.step.dependOn(&emSdkStep.step);
            }

            // console_engine_lib.addIncludePath(dep.path("upstream/emscripten/cache/sysroot/include"));
            console_engine_lib.addSystemIncludePath(dep.path("upstream/emscripten/cache/sysroot/include"));
            // std.debug.print("{s}\n", .{dep.path("upstream/emscripten/cache/sysroot/include").getPath(b)});
            // @panic("Debug");
        } else {
            @panic("Can't fetch emscripten");
        }
    }
    console_engine_lib.linkLibrary(raylib);
    console_engine_lib.linkLibCpp();
    try addAllCppSources(b, console_engine_lib.root_module, "ConsoleEngine/");

    console_engine_lib.addIncludePath(b.path("ConsoleEngine"));
    console_engine_lib.installLibraryHeaders(raylib);
    console_engine_lib.installHeadersDirectory(
        b.path("ConsoleEngine/"),
        "ConsoleEngine",
        .{},
    );
    console_engine_lib.installHeader(
        b.path("ConsoleEngine/include/ConsoleGame.h"),
        "ConsoleGame.h",
    );

    return console_engine_lib;
}

fn getRaylib(
    b: *std.Build,
    target: std.Build.ResolvedTarget,
    optimize: std.builtin.OptimizeMode,
    options: rl.Options,
) *std.Build.Step.Compile {
    const raylib_dep = b.dependency("raylib", .{
        .target = target,
        .optimize = optimize,
        .raudio = true,
        .rmodels = false,
        .rshapes = true,
        .rtext = false,
        .rtextures = true,
        .platform = options.platform,
        .shared = options.shared,
        .linux_display_backend = options.linux_display_backend,
        .opengl_version = options.opengl_version,
        .android_api_version = options.android_api_version,
        .android_ndk = options.android_ndk,
    });

    const raylib = raylib_dep.artifact("raylib");

    b.installArtifact(raylib);
    return raylib;
}

pub fn addAllCppSources(
    b: *std.Build,
    mod: *std.Build.Module,
    dir_path: []const u8,
) !void {
    var dir = try std.fs.cwd().openDir(
        dir_path,
        .{ .iterate = true },
    );
    var walker = try dir.walk(b.allocator);
    defer walker.deinit();

    const allowed_exts = [_][]const u8{".cpp"};
    const excluded_paths = [_][]const u8{"resource"};

    while (try walker.next()) |entry| {
        const ext = std.fs.path.extension(entry.basename);
        const include_file = for (allowed_exts) |e| {
            if (std.mem.eql(u8, ext, e))
                break true;
        } else false;
        const excluded = for (excluded_paths) |e| {
            if (std.mem.startsWith(u8, ext, e))
                break true;
        } else false;

        if (include_file and !excluded) {
            mod.addCSourceFile(.{
                .file = b.path(b.pathJoin(&.{ dir_path, entry.path })),
                .language = .cpp,
                .flags = &.{"-std=c++20"},
            });
        }
    }
}

fn createEmsdkStep(b: *std.Build, emsdk: *std.Build.Dependency) *std.Build.Step.Run {
    if (builtin.os.tag == .windows) {
        return b.addSystemCommand(&.{emsdk.path("emsdk.bat").getPath(b)});
    } else {
        return b.addSystemCommand(&.{emsdk.path("emsdk").getPath(b)});
    }
}

fn emSdkSetupStep(b: *std.Build, emsdk: *std.Build.Dependency) !?*std.Build.Step.Run {
    const dot_emsc_path = emsdk.path(".emscripten").getPath(b);
    const dot_emsc_exists = !std.meta.isError(std.fs.accessAbsolute(dot_emsc_path, .{}));

    if (!dot_emsc_exists) {
        const emsdk_install = createEmsdkStep(b, emsdk);
        emsdk_install.addArgs(&.{ "install", "latest" });
        const emsdk_activate = createEmsdkStep(b, emsdk);
        emsdk_activate.addArgs(&.{ "activate", "latest" });
        emsdk_activate.step.dependOn(&emsdk_install.step);
        return emsdk_activate;
    } else {
        return null;
    }
}

// Adapted from Not-Nik/raylib-zig
fn emscriptenRunStep(b: *std.Build, emsdk: *std.Build.Dependency, examplePath: []const u8) !*std.Build.Step.Run {
    const dot_emsc_path = emsdk.path("upstream/emscripten/cache/sysroot/include").getPath(b);
    // If compiling on windows , use emrun.bat.
    const emrunExe = switch (builtin.os.tag) {
        .windows => "emrun.bat",
        else => "emrun",
    };
    var emrun_run_arg = try b.allocator.alloc(u8, dot_emsc_path.len + emrunExe.len + 1);
    defer b.allocator.free(emrun_run_arg);

    if (b.sysroot == null) {
        emrun_run_arg = try std.fmt.bufPrint(emrun_run_arg, "{s}", .{emrunExe});
    } else {
        emrun_run_arg = try std.fmt.bufPrint(emrun_run_arg, "{s}" ++ std.fs.path.sep_str ++ "{s}", .{ dot_emsc_path, emrunExe });
    }
    const run_cmd = b.addSystemCommand(&.{ emrun_run_arg, examplePath });
    return run_cmd;
}
