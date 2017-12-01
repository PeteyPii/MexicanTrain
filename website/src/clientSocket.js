import io from 'socket.io-client';
const socket = io({ path: '/mexican-train/socket.io/' });
socket.on('message', function(data) {
  console.log('WS message:', data);
});
export default socket;
