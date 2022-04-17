# Linda

## Quick start

### Ubuntu

```sh
sudo apt-get update
sudo apt-get install cmake ninja-build
sudo make prepare build run
```

### Other systems

1. [Install Cmake](https://cgold.readthedocs.io/en/latest/first-step/installation.html)

2. [Install Ninja](https://ninja-build.org)

3. Run Make command

    ```sh
    sudo make prepare build test run
    ```

## Others

- in order to run application you need to be a superuser. It's necessary for the communication via FIFO.
- Pipes are stored at: /tmp/fifos
- testing framework: [doctest](https://github.com/doctest/doctest)
