const std = @import("std");
const rl = @import("raylib");

pub const Options = rl.Options;
pub const OpenglVersion = rl.OpenglVersion;
pub const LinuxDisplayBackend = rl.LinuxDisplayBackend;
pub const PlatformBackend = rl.PlatformBackend;

pub fn build(b: *std.Build) !void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const console_engine_lib = try getConsoleGameLib(b, target, optimize);

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
    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);
}

fn getConsoleGameLib(b: *std.Build, target: std.Build.ResolvedTarget, optimize: std.builtin.OptimizeMode) !*std.Build.Step.Compile {
    const raylib = getRaylib(b, target, optimize, Options.getOptions(b));

    var console_engine_lib = b.addStaticLibrary(.{
        .name = "ConsoleEngine",
        .target = target,
        .optimize = optimize,
    });
    console_engine_lib.linkLibCpp();
    console_engine_lib.linkLibrary(raylib);
    try addAllCppSources(b, console_engine_lib.root_module, "ConsoleEngine/");
    // try installAllHeaders(b, console_engine_lib, "ConsoleEngine/");
    console_engine_lib.addIncludePath(b.path("ConsoleEngine"));
    console_engine_lib.installLibraryHeaders(raylib);
    console_engine_lib.installHeadersDirectory(b.path("ConsoleEngine/"), "ConsoleEngine", .{});
    console_engine_lib.installHeader(b.path("ConsoleEngine/include/ConsoleGame.h"), "ConsoleGame.h");

    return console_engine_lib;
}

fn getRaylib(b: *std.Build, target: std.Build.ResolvedTarget, optimize: std.builtin.OptimizeMode, options: Options) *std.Build.Step.Compile {
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

pub fn addAllCppSources(b: *std.Build, mod: *std.Build.Module, dir_path: []const u8) !void {
    var dir = try std.fs.cwd().openDir(dir_path, .{ .iterate = true });
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
