
import { Prop, Schema, SchemaFactory } from '@nestjs/mongoose';
import { HydratedDocument } from 'mongoose';

export type PathDocument = HydratedDocument<Path>;

@Schema()
export class Path {

  @Prop()
  values: number[];

}

export const PathSchema = SchemaFactory.createForClass(Path);