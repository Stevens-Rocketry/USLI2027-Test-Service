# USLI2027-Test-Service

Test systemd service (`tsservice`) that only logs its state. Used to check that the payload image and the command & control service can start, reload and stop services.

## Requirements

- libsystemd (Arch: `systemd-libs`, Debian/Ubuntu: `libsystemd-dev`)
- gcc, make

## Build

```sh
make
```

## Install and run

```sh
sudo install -m 0755 tsservice /usr/bin/tsservice
sudo install -m 0644 test.service /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl enable --now test
```

## Test

```sh
systemctl reload test     # should log a reload and return to active
systemctl stop test       # should log "Shutting down..."
journalctl -u test -f     # view logs
```

Based on systemd's `notify-selfcontained-example.c` (MIT-0).
