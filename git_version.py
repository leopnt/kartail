import subprocess
import os


def get_git_version():
    try:
        git_version = (
            subprocess.check_output(["git", "rev-parse", "--short", "HEAD"])
            .decode("utf-8")
            .strip()
        )
    except Exception:
        git_version = "unknown"
    return git_version


version_header = os.path.join("include", "git_version.h")

with open(version_header, "w") as f:
    f.write(f'#define GIT_VERSION "{get_git_version()}"\n')

print(f"Generated {version_header}")
