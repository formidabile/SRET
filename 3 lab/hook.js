

(function () {



    // @ts-ignore
    function hook_native_addr(funcPtr, paramsNum) {//первый аргумент адрес хукаемой функции, второй аргумент - число аргументов в этой функции
        var module = Process.findModuleByAddress(funcPtr);
        try {
            Interceptor.attach(funcPtr, {
                onEnter: function (args) {//манипуляции при вызове функции
                    this.logs = "";
                    // @ts-ignore
                    this.logs=this.logs.concat("So: " + module.name + "  Method: SSL_read offset: " + ptr(funcPtr).sub(module.base) + "\n");
                    this.buf=args[1];// сохраняем содержимое второго аргумента, буфер в который выводится расшифрованное сообщение
                }, onLeave: function (retval) {//манипулции при выходе из функции
                    this.logs=this.logs.concat("buf onLeave: " + hexdump(this.buf) + "\n");//выводим в лог дамп буфера
                    console.log(this.logs);
                }
            });
        } catch (e) {
            console.log(e);
        }
    }
    // @ts-ignore
    hook_native_addr(Module.findBaseAddress("chrome.dll").add(0x43caf10), 0x3);//вычисляем адрес базы библиотеки и прибавляем к ней адрес функции
})();

