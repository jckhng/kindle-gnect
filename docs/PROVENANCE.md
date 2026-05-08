# Provenance And Licensing

Exact Four in a Row is an unofficial Kindle-focused derivative/adaptation of GNOME
Games' Gnect.

It is also informed by the GnomeGames4Kindle porting work. Original GNOME
Games / Gnect code, ideas, and artwork remain credited to their upstream
authors. Kindle porting groundwork and packaging references are credited to
GnomeGames4Kindle, originally by crazy-electron, and later contributors.

This repository is not an official GNOME project and is not an official
GnomeGames4Kindle release.

## What Comes From GNOME Games / Gnect

- Gnect/Connect Four game identity, design lineage, and project inspiration.
- Gnect visual assets copied into `assets/`.
- Project licensing basis in `licenses/`.

## What Is Kindle-Specific

- Compact GTK2/Cairo Kindle interface in `main.c`.
- Small standalone Connect Four rules/AI implementation in `gnect_engine.c`.
- KUAL extension files in `extension/`.
- Docker ARM build and release packaging scripts.
- Runtime-library bundling for easier Kindle installation.
- Release packaging and Kindle launch scripts maintained for this derivative.

## License Notes

The GNOME-derived assets and project lineage keep their original GPL-family
licensing. The license texts included with this repository are in:

```text
licenses/
```

The release zip also bundles shared runtime libraries from Debian Bullseye ARM.
Those libraries keep their own upstream licenses. The generated extension
package includes:

```text
extensions/exact-four-in-a-row/LICENSES/RUNTIME-LIBS.txt
extensions/exact-four-in-a-row/LICENSES/THIRD-PARTY-NOTICE.txt
```

If publishing binary releases, keep the license files and runtime notices with
the package.
