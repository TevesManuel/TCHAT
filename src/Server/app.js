const net = require('net');

const clients_sockets = new Map();

function getSocketKey(socket)
{
    return socket.remoteAddress + ":" + socket.remotePort;
}

const server = net.createServer((socket) => {
    console.log('Cliente conectado:', socket.remoteAddress, socket.remotePort);
    socket.write(`Connection Server Confirmation.`);
    clients_sockets.set(getSocketKey(socket), {socket: socket, name: null, party: null});
    socket.on('data', (data) =>
    {
        // console.log(`Datos recibidos del cliente (${getSocketKey(socket)}): ${data.toString('utf-8')}`);

        if(data.toString().includes("_TEVESREGISTER_"))
        {
            console.log("Register " + getSocketKey(socket) + " to \"" +
            data.toString().replace("_TEVESREGISTER_", "").split("_TEVESSEPARATOR_")[1] + "\" party with \"" +
            data.toString().replace("_TEVESREGISTER_", "").split("_TEVESSEPARATOR_")[0] + "\" name.");
            clients_sockets.get(getSocketKey(socket)).name =  data.toString().replace("_TEVESREGISTER_", "").split("_TEVESSEPARATOR_")[0];
            clients_sockets.get(getSocketKey(socket)).party =  data.toString().replace("_TEVESREGISTER_", "").split("_TEVESSEPARATOR_")[1];
            let str = "At the party are ";
            let istr = str.length;
            let iusrs = 0;
            clients_sockets.forEach((client_struct, _) => {
                if(getSocketKey(client_struct.socket) != getSocketKey(socket))
                {
                    if(client_struct.party == clients_sockets.get(getSocketKey(socket)).party)
                    {
                        client_struct.socket.write(clients_sockets.get(getSocketKey(socket)).name + " connected.");
                        if(iusrs == 0)
                            str += client_struct.name;
                        else
                            str += ", " + client_struct.name;
                        iusrs++;   
                    }
                }
            });
            if(istr == str.length)
                str = "You are alone at the party.";
            else
            {
                var i = str.lastIndexOf(',');
                if(i !== -1)
                    str = str.substring(0, i) + ' and' + str.substring(i + 1);
                str += ".";
            }
            socket.write(str);
        }
        else
        {
            // console.log("message \"" + data.toString('utf-8').split("_TEVESSEPARATOR_")[2] + "\" from " + getSocketKey(socket));
            clients_sockets.forEach((client_struct, _) => {
                if(getSocketKey(client_struct.socket) != getSocketKey(socket))
                {
                    if(client_struct.party == clients_sockets.get(getSocketKey(socket)).party)
                        client_struct.socket.write(clients_sockets.get(getSocketKey(socket)).name + ":" + data.toString('utf-8'));

                    // console.log("Sending message \"", data.toString('utf-8'), "\" To: ", socket.remoteAddress, ":", socket.remotePort)    
                }
            });        
        }
        // socket.write(`Servidor: Recibí tus datos: ${data}`);
    });
    let disconnect_function = () => {
        clients_sockets.forEach((client_struct, _) => {
            if(getSocketKey(client_struct.socket) != getSocketKey(socket))
            {
                if(client_struct.party == clients_sockets.get(getSocketKey(socket)).party)
                {
                    client_struct.socket.write(clients_sockets.get(getSocketKey(socket)).name + " disconnected.");
                }
            }
        });
        console.log('Cliente desconectado:', socket.remoteAddress, socket.remotePort);
        clients_sockets.delete(getSocketKey(socket));
    };
    socket.on('end', disconnect_function);
    socket.on('error', disconnect_function);
});

const PORT = 3000;

server.listen(PORT, () => {
    console.log(`Servidor TCP escuchando en el puerto ${PORT}`);
});