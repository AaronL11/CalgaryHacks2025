use std::io::{prelude::*,BufReader};
use std::net::{TcpListener,SocketAddrV4,Ipv4Addr,TcpStream};


fn main() -> std::io::Result<()> {
    let addr = SocketAddrV4::new(Ipv4Addr::new(10,13,68,31),8000);
    println!("Booting up server");
    let listener = TcpListener::bind(addr)?;

    println!("listening on: {addr}");
    loop {
        let (stream,strm_addr) = listener.accept()?;
        println!("connected to address: {strm_addr}");
        handle(stream)?;
        println!("Connection closed with {addr}\n");
    }
    Ok(())
}

fn handle(mut stream: TcpStream) -> std::io::Result<()> {
    let mut buf_reader = BufReader::new(&stream);
    let mut buf = vec![0;10];

    buf_reader.read(&mut  buf[..])?;
    println!("Message received: {}", String::from_utf8(buf).unwrap());
    let msg = "hai";
    stream.write_all(msg.as_bytes())?;
    println!("Message sent: {msg}");
    Ok(())
}
