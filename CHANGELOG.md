# Changelog

All notable changes to this project will be documented in this file.

The format is based on **Keep a Changelog**  
and this project adheres to **Semantic Versioning (SemVer)**.

---

## [Unreleased]
## [0.1.0] - 2025-12-18

### Added
- 

### Changed
- 

### Removed
- 

## [0.0.2] - 2025-12-12

### Added
- 

### Changed
- 

### Removed
- 

## [0.0.1] - 2025-12-11

### Added
- 

### Changed
- 

### Removed
- 


### Added

- Initial project structure and repository architecture.
- Base CMake configuration with presets for dev/prod.
- Draft API documentation (`docs/api.md`).
- Core vision, architecture, and storage model in `docs/`.
- Configuration files: dev, prod, and example local config.
- Database migrations:
  - `0001_init_schema.sql` — packages, versions, users.
  - `0002_add_tokens.sql` — API tokens and scopes.
- Scripts for development workflow (`dev.sh`, `migrate.sh`, `seed.sh`, `changelog-release.sh`, `update_changelog.sh`).
- Initial storage system:
  - `LocalFileStorage` implementation.
  - S3 storage stub for future cloud integration.
- HTTP server structure with routes and middleware.
- Domain models: Package, Version, User, Token, Errors.
- Service layer (Auth, Package, Version).
- Repository layer (PackageRepository, UserRepository, Database).
- Minimal working `main.cpp` with App bootstrap.
- Integration and unit tests scaffolding.
- Example clients (Python + Vix client).
- Example cURL API usage.

### Changed

- Improved `.gitignore` with full C++ and project-specific exclusions.

### Removed

- `cmd.md` removed due to sensitive content.
