# /dev/no

A device for those times. And those people. You know them.

I'm not [the first](https://github.com/kemenril/devno), but I wanted to
implement this myself.

**Limitation of Liability:** To the maximum extent permitted by
applicable law, in no event shall the authors, contributors, or
copyright holders of the Software be held liable for any direct,
indirect, incidental, special, exemplary, or consequential damages,
including but not limited to procurement of substitute goods or
services; loss of use, data, or profits; or business interruption,
however caused and on any theory of liability, whether in contract,
strict liability, or tort (including negligence or otherwise) arising
in any way out of the use of the Software, even if advised of the
possibility of such damage.

**Assumption of Risk:** By using the Software, you agree to assume all
risks associated with its installation and operation, including but not
limited to potential system crashes, data loss, or any other adverse
impacts on system performance.

## Building

On Arch Linux, have `linux-headers` installed and run

```sh
make
```

You can then load the module like this:

```sh
modprobe ./dev_no.ko
```

Once you have loaded the module, you will be able to read the `/dev/no`
file.

```sh
cat /dev/no
```
