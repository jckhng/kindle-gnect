# Release Notes

## Current Release

Artifact:

```text
release/kindle-gnect-extension.zip
```

Screenshot:

```text
screenshots/kindle-gnect.png
```

Verify:

```bash
cd release
sha256sum -c SHA256SUMS
```

Current checksum:

```text
5b5e0b7895b68d5dff297fd969c54e8a638e7ee11535b691d4555f1840d035f3  kindle-gnect-extension.zip
```

Contents:

- ARM hard-float `kindle-gnect` executable.
- KUAL extension metadata and launch scripts.
- Gnect visual assets copied from GNOME Games.
- Bundled GTK2/Cairo runtime library set copied from the ARM Docker builder.
- License and third-party runtime notices.

Known constraints:

- This is an unofficial derivative/adaptation release, not an official GNOME or
  GnomeGames4Kindle release.
- Requires a jailbroken Kindle with KUAL.
- Kindle home-screen `.sh` tapping is not reliable unless another launcher/file
  association is installed. Use KUAL.

GitHub release upload:

- Upload `release/kindle-gnect-extension.zip` as the binary asset.
- Include the checksum from `release/SHA256SUMS` in the release notes.
- Keep the source repository public with `licenses/`, `docs/`, and `assets/`
  intact so recipients can inspect the derivative-work provenance.
