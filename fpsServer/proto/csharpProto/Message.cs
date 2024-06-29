// <auto-generated>
//     Generated by the protocol buffer compiler.  DO NOT EDIT!
//     source: message.proto
// </auto-generated>
#pragma warning disable 1591, 0612, 3021
#region Designer generated code

using pb = global::Google.Protobuf;
using pbc = global::Google.Protobuf.Collections;
using pbr = global::Google.Protobuf.Reflection;
using scg = global::System.Collections.Generic;
/// <summary>Holder for reflection information generated from message.proto</summary>
public static partial class MessageReflection {

  #region Descriptor
  /// <summary>File descriptor for message.proto</summary>
  public static pbr::FileDescriptor Descriptor {
    get { return descriptor; }
  }
  private static pbr::FileDescriptor descriptor;

  static MessageReflection() {
    byte[] descriptorData = global::System.Convert.FromBase64String(
        string.Concat(
          "Cg1tZXNzYWdlLnByb3RvIlUKCFBvc2l0aW9uEgkKAXgYASABKAISCQoBeRgC",
          "IAEoAhIJCgF6GAMgASgCEgsKA3lhdxgEIAEoAhINCgVwaXRjaBgFIAEoAhIM",
          "CgRyb2xsGAYgASgCKloKCUNvbW1hbmRJRBIPCgtDSURfVU5LTk9XThAAEhIK",
          "DUNJRF9MT0dJTl9SRVEQgQISEgoNQ0lEX0xPR0lOX1JFUxCCAhIUCg9DSURf",
          "UExBWUVSX01PVkUQgwJiBnByb3RvMw=="));
    descriptor = pbr::FileDescriptor.FromGeneratedCode(descriptorData,
        new pbr::FileDescriptor[] { },
        new pbr::GeneratedClrTypeInfo(new[] {typeof(global::CommandID), }, null, new pbr::GeneratedClrTypeInfo[] {
          new pbr::GeneratedClrTypeInfo(typeof(global::Position), global::Position.Parser, new[]{ "X", "Y", "Z", "Yaw", "Pitch", "Roll" }, null, null, null, null)
        }));
  }
  #endregion

}
#region Enums
public enum CommandID {
  [pbr::OriginalName("CID_UNKNOWN")] CidUnknown = 0,
  [pbr::OriginalName("CID_LOGIN_REQ")] CidLoginReq = 257,
  [pbr::OriginalName("CID_LOGIN_RES")] CidLoginRes = 258,
  [pbr::OriginalName("CID_PLAYER_MOVE")] CidPlayerMove = 259,
}

#endregion

#region Messages
public sealed partial class Position : pb::IMessage<Position> {
  private static readonly pb::MessageParser<Position> _parser = new pb::MessageParser<Position>(() => new Position());
  private pb::UnknownFieldSet _unknownFields;
  [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
  public static pb::MessageParser<Position> Parser { get { return _parser; } }

  [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
  public static pbr::MessageDescriptor Descriptor {
    get { return global::MessageReflection.Descriptor.MessageTypes[0]; }
  }

  [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
  pbr::MessageDescriptor pb::IMessage.Descriptor {
    get { return Descriptor; }
  }

  [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
  public Position() {
    OnConstruction();
  }

  partial void OnConstruction();

  [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
  public Position(Position other) : this() {
    x_ = other.x_;
    y_ = other.y_;
    z_ = other.z_;
    yaw_ = other.yaw_;
    pitch_ = other.pitch_;
    roll_ = other.roll_;
    _unknownFields = pb::UnknownFieldSet.Clone(other._unknownFields);
  }

  [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
  public Position Clone() {
    return new Position(this);
  }

  /// <summary>Field number for the "x" field.</summary>
  public const int XFieldNumber = 1;
  private float x_;
  [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
  public float X {
    get { return x_; }
    set {
      x_ = value;
    }
  }

  /// <summary>Field number for the "y" field.</summary>
  public const int YFieldNumber = 2;
  private float y_;
  [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
  public float Y {
    get { return y_; }
    set {
      y_ = value;
    }
  }

  /// <summary>Field number for the "z" field.</summary>
  public const int ZFieldNumber = 3;
  private float z_;
  [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
  public float Z {
    get { return z_; }
    set {
      z_ = value;
    }
  }

  /// <summary>Field number for the "yaw" field.</summary>
  public const int YawFieldNumber = 4;
  private float yaw_;
  [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
  public float Yaw {
    get { return yaw_; }
    set {
      yaw_ = value;
    }
  }

  /// <summary>Field number for the "pitch" field.</summary>
  public const int PitchFieldNumber = 5;
  private float pitch_;
  [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
  public float Pitch {
    get { return pitch_; }
    set {
      pitch_ = value;
    }
  }

  /// <summary>Field number for the "roll" field.</summary>
  public const int RollFieldNumber = 6;
  private float roll_;
  [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
  public float Roll {
    get { return roll_; }
    set {
      roll_ = value;
    }
  }

  [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
  public override bool Equals(object other) {
    return Equals(other as Position);
  }

  [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
  public bool Equals(Position other) {
    if (ReferenceEquals(other, null)) {
      return false;
    }
    if (ReferenceEquals(other, this)) {
      return true;
    }
    if (!pbc::ProtobufEqualityComparers.BitwiseSingleEqualityComparer.Equals(X, other.X)) return false;
    if (!pbc::ProtobufEqualityComparers.BitwiseSingleEqualityComparer.Equals(Y, other.Y)) return false;
    if (!pbc::ProtobufEqualityComparers.BitwiseSingleEqualityComparer.Equals(Z, other.Z)) return false;
    if (!pbc::ProtobufEqualityComparers.BitwiseSingleEqualityComparer.Equals(Yaw, other.Yaw)) return false;
    if (!pbc::ProtobufEqualityComparers.BitwiseSingleEqualityComparer.Equals(Pitch, other.Pitch)) return false;
    if (!pbc::ProtobufEqualityComparers.BitwiseSingleEqualityComparer.Equals(Roll, other.Roll)) return false;
    return Equals(_unknownFields, other._unknownFields);
  }

  [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
  public override int GetHashCode() {
    int hash = 1;
    if (X != 0F) hash ^= pbc::ProtobufEqualityComparers.BitwiseSingleEqualityComparer.GetHashCode(X);
    if (Y != 0F) hash ^= pbc::ProtobufEqualityComparers.BitwiseSingleEqualityComparer.GetHashCode(Y);
    if (Z != 0F) hash ^= pbc::ProtobufEqualityComparers.BitwiseSingleEqualityComparer.GetHashCode(Z);
    if (Yaw != 0F) hash ^= pbc::ProtobufEqualityComparers.BitwiseSingleEqualityComparer.GetHashCode(Yaw);
    if (Pitch != 0F) hash ^= pbc::ProtobufEqualityComparers.BitwiseSingleEqualityComparer.GetHashCode(Pitch);
    if (Roll != 0F) hash ^= pbc::ProtobufEqualityComparers.BitwiseSingleEqualityComparer.GetHashCode(Roll);
    if (_unknownFields != null) {
      hash ^= _unknownFields.GetHashCode();
    }
    return hash;
  }

  [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
  public override string ToString() {
    return pb::JsonFormatter.ToDiagnosticString(this);
  }

  [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
  public void WriteTo(pb::CodedOutputStream output) {
    if (X != 0F) {
      output.WriteRawTag(13);
      output.WriteFloat(X);
    }
    if (Y != 0F) {
      output.WriteRawTag(21);
      output.WriteFloat(Y);
    }
    if (Z != 0F) {
      output.WriteRawTag(29);
      output.WriteFloat(Z);
    }
    if (Yaw != 0F) {
      output.WriteRawTag(37);
      output.WriteFloat(Yaw);
    }
    if (Pitch != 0F) {
      output.WriteRawTag(45);
      output.WriteFloat(Pitch);
    }
    if (Roll != 0F) {
      output.WriteRawTag(53);
      output.WriteFloat(Roll);
    }
    if (_unknownFields != null) {
      _unknownFields.WriteTo(output);
    }
  }

  [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
  public int CalculateSize() {
    int size = 0;
    if (X != 0F) {
      size += 1 + 4;
    }
    if (Y != 0F) {
      size += 1 + 4;
    }
    if (Z != 0F) {
      size += 1 + 4;
    }
    if (Yaw != 0F) {
      size += 1 + 4;
    }
    if (Pitch != 0F) {
      size += 1 + 4;
    }
    if (Roll != 0F) {
      size += 1 + 4;
    }
    if (_unknownFields != null) {
      size += _unknownFields.CalculateSize();
    }
    return size;
  }

  [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
  public void MergeFrom(Position other) {
    if (other == null) {
      return;
    }
    if (other.X != 0F) {
      X = other.X;
    }
    if (other.Y != 0F) {
      Y = other.Y;
    }
    if (other.Z != 0F) {
      Z = other.Z;
    }
    if (other.Yaw != 0F) {
      Yaw = other.Yaw;
    }
    if (other.Pitch != 0F) {
      Pitch = other.Pitch;
    }
    if (other.Roll != 0F) {
      Roll = other.Roll;
    }
    _unknownFields = pb::UnknownFieldSet.MergeFrom(_unknownFields, other._unknownFields);
  }

  [global::System.Diagnostics.DebuggerNonUserCodeAttribute]
  public void MergeFrom(pb::CodedInputStream input) {
    uint tag;
    while ((tag = input.ReadTag()) != 0) {
      switch(tag) {
        default:
          _unknownFields = pb::UnknownFieldSet.MergeFieldFrom(_unknownFields, input);
          break;
        case 13: {
          X = input.ReadFloat();
          break;
        }
        case 21: {
          Y = input.ReadFloat();
          break;
        }
        case 29: {
          Z = input.ReadFloat();
          break;
        }
        case 37: {
          Yaw = input.ReadFloat();
          break;
        }
        case 45: {
          Pitch = input.ReadFloat();
          break;
        }
        case 53: {
          Roll = input.ReadFloat();
          break;
        }
      }
    }
  }

}

#endregion


#endregion Designer generated code
