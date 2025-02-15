use std::io::{prelude::*,BufReader};
use std::net::{TcpListener,SocketAddrV4,Ipv4Addr,TcpStream};


fn main() -> std::io::Result<()> {
    let addr = SocketAddrV4::new(Ipv4Addr::new(127,0,0,0),8000);
    let listener = TcpListener::bind(addr)?;

    let (stream,strm_addr) = listener.accept()?;
    handle(stream);

    Ok(())
}

fn handle(mut stream: TcpStream) {
    let buf_reader = BufReader::new(&stream);
}