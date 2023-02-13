
<style type="text/css">
.tableStyle th, .tableStyle td {
    padding: 9px;
}
</style>

<table class="tableStyle" style="margin:0; text-align: center">
<caption>BTD6 Multiplayer packet header</caption>
<tbody>
<tr>
<th style="border-right:none; border-bottom:none;">Offset</th>
<th colspan="1" style="width: 22%">Byte + 0</th>
<th colspan="1" style="width: 22%">Byte + 1</th>
<th colspan="1" style="width: 22%">Byte + 2</th>
<th colspan="1" style="width: 22%">Byte + 3</th>
</tr>
<tr>
<th>Bytes<br>0-4</th>
<td colspan="4" style="border-bottom:none">Magic Number</td>
</tr>
<tr>
<th>Bytes<br>5-8</th>
<td colspan="1" style="border-top:none">Magic Number</td>
<td colspan="1">Version</td>
<td colspan="2">Packet Length</td>
</tr>
<tr>
<th>Bytes<br>9-12</th>
<td colspan="4">Packet Type Name Length</td>
</tr>
<tr>
<th>Bytes<br>13-(n-1)</th>
<td colspan="4">Packet Type Name</td>
</tr>
</tbody>
</table>

### Magic Number (6 Bytes)
The magic number is the bytes "4E 4B 4D 6C 74" or "NKMlt".

### Version (1 Byte)
The version is currently always 1.

### Packet Length (2 Bytes)
The length of the packet not including the Magic Number or the delimiter in Little-Endian format.

### Packet Type Length (4 Bytes)
The length of the name of the type of the packet in bytes e.g. "ECHOMessage" is 04 00 00 00.
<br>
This can be 0.

### Packet Type (Packet Type Length Bytes)
The type of the packet. Used for when players connect and disconnect. 

<table class="tableStyle" style="margin:0; text-align: center">
<caption>Packet Types</caption>
<tbody>
<tr>
<th>Code</th>
<th>Type</th>
</tr>
<tr>
<td>JSM</td>
<td>Join Session Message</td>
</tr>
<tr>
<td>JSRM</td>
<td>Join Session Response Message</td>
</tr>
<tr>
<td>ECHOMessage</td>
<td>Latency Test Message</td>
</tr>
<tr>
<td>ECHR</td>
<td>Latency Report Message</td>
</tr>
<tr>
<td>PCM</td>
<td>Peer Connected Message</td>
</tr>
<tr>
<td>DC</td>
<td>Peer Disconnected Message</td>
</tr>
<tr>
<td>TRG</td>
<td>Peer Targeted Message</td>
</tr>
<tr>
<td>None</td>
<td>Event data packet</td>
</tr>
</tbody>
</table>


