using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TeensyFFComms.Protocols;

namespace TeensyFFComms
{
    public interface F3Message
    {
        F3Opcode Opcode { get; set; }
    }
}
